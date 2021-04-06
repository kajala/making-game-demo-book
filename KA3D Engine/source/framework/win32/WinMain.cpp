// Simple application framework
#include <io/PathName.h>
#include <io/FindFile.h>
#include <io/FileInputStream.h>
#include <gr/dx/DX_Context.h>
#include <hgr/Globals.h>
#include <hgr/ParticleSystem.h>
#include <hgr/DefaultResourceManager.h>
#include <lang/Debug.h>
#include <lang/System.h>
#include <lang/Globals.h>
#include <lang/Exception.h>
#include <framework/App.h>
#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include <crtdbg.h>
#include <config.h>


using namespace io;
using namespace gr;
using namespace hgr;
using namespace lang;
using namespace framework;


static const int s_keyToVk[] =
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	VK_NEXT,
	VK_PRIOR,
	VK_HOME,
	VK_END,
	VK_INSERT,
	VK_DELETE,
	VK_RETURN,
	VK_SHIFT,
	VK_CONTROL,
	VK_TAB,
	VK_SPACE,
	VK_BACK,
	VK_PAUSE,
	VK_ESCAPE,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	-1,
	-1,
	-1
};

static P(DX_Context)		s_context			= 0;
static bool					s_active			= false;
static Array<String>		s_shaderfullnames;
static Array<String>		s_shaderbasenames;
static Array<String>		s_particlefullnames;
static Array<String>		s_particlebasenames;
static HWND					s_hwnd;
static bool					s_captureMouse;


static void deinit()
{
	if ( App::initialized() )
	{
		App::deleteApp();
		hgr::Globals::cleanup();
		assert( 1 == s_context->references() );
		s_context = 0;
		DestroyWindow( s_hwnd );
		s_hwnd = 0;
	}
}

static LRESULT wndproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
	App* a = 0;
	if ( App::initialized() )
		a = App::get();

	switch ( msg )
	{
	case WM_SETCURSOR:
		if ( s_captureMouse )
		{
			SetCursor( 0 );
			return TRUE;
		}
		break;

	case WM_CLOSE:
		deinit();
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
		if ( a != 0 )
		{
			a->App::keyDown( App::KEY_LBUTTON );
			a->keyDown( App::KEY_LBUTTON );
		}
		break;
	case WM_MBUTTONDOWN:
		if ( a != 0 )
		{
			a->App::keyDown( App::KEY_MBUTTON );
			a->keyDown( App::KEY_MBUTTON );
		}
		break;
	case WM_RBUTTONDOWN:
		if ( a != 0 )
		{
			a->App::keyDown( App::KEY_RBUTTON );
			a->keyDown( App::KEY_RBUTTON );
		}
		break;

	case WM_LBUTTONUP:
		if ( a != 0 )
		{
			a->App::keyUp( App::KEY_LBUTTON );
			a->keyUp( App::KEY_LBUTTON );
		}
		break;
	case WM_MBUTTONUP:
		if ( a != 0 )
		{
			a->App::keyUp( App::KEY_MBUTTON );
			a->keyUp( App::KEY_MBUTTON );
		}
		break;
	case WM_RBUTTONUP:
		if ( a != 0 )
		{
			a->App::keyUp( App::KEY_RBUTTON );
			a->keyUp( App::KEY_RBUTTON );
		}
		break;

	case WM_MOUSEMOVE:
		if ( a != 0 )
		{
			int cx = 0;
			int cy = 0;

			if ( s_captureMouse )
			{
				RECT rc;
				GetClientRect( hwnd, &rc );
				cx = rc.right/2;
				cy = rc.bottom/2;
			}

			POINT p;
			p.x = short(lp&0xFFFF);
			p.y = short((lp>>16)&0xFFFF);
			App::get()->App::mouseMove( p.x-cx, p.y-cy );
			App::get()->mouseMove( p.x-cx, p.y-cy );
		}
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
		if ( a != 0 )
		{
			int vk = (int)wp;
			bool keydown = WM_KEYDOWN==msg;
			assert( sizeof(s_keyToVk)/sizeof(s_keyToVk[0]) == App::KEY_COUNT );

			for ( int i = 0 ; i < App::KEY_COUNT ; ++i )
			{
				if ( s_keyToVk[i] == vk )
				{
					if ( keydown )
					{
						a->keyDown( (App::KeyType)i );
						a->App::keyDown( (App::KeyType)i );
					}
					else
					{
						a->keyUp( (App::KeyType)i );
						a->App::keyUp( (App::KeyType)i );
					}
					break;
				}
			}
		}
		break;

	case WM_ACTIVATE:{
		s_active = ( LOWORD(wp) != WA_INACTIVE );

		char name[256];
		GetWindowText( hwnd, name, sizeof(name) );
		Debug::printf( "framework: Window \"%s\" %s\n", name, (s_active?"activated":"deactivated") );

		if ( App::initialized() )
		{
			App::get()->App::activate( s_active );
			App::get()->activate( s_active );
		}
		break;}
	}

	return DefWindowProc( hwnd, msg, wp, lp );
}

static HWND createMainWindow( const char* classname, const char* name,
	int w, int h, bool fullscreen, HINSTANCE instance, int iconresid )
{
	DWORD style = WS_VISIBLE|WS_OVERLAPPEDWINDOW;
	DWORD exstyle = 0;
	if ( fullscreen )
	{
		style = WS_VISIBLE|WS_POPUP;
		exstyle = WS_EX_TOPMOST;
	}

	HWND parenthwnd = 0;
	HICON icon = 0;
	if ( iconresid > 0 )
		icon = LoadIcon( instance, MAKEINTRESOURCE(iconresid) );

	WNDCLASSEXA wndclassex;
	wndclassex.cbSize			= sizeof(WNDCLASSEXA); 
	wndclassex.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc		= (WNDPROC)wndproc;
	wndclassex.cbClsExtra		= 0;
	wndclassex.cbWndExtra		= 0;
	wndclassex.hInstance		= (HINSTANCE)instance;
	wndclassex.hIcon			= icon;
	wndclassex.hCursor			= s_captureMouse ? 0 : LoadCursor(NULL, IDC_ARROW);
	wndclassex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclassex.lpszMenuName		= 0;
	wndclassex.lpszClassName	= classname;
	wndclassex.hIconSm			= 0;
	bool ok = RegisterClassExA(&wndclassex) != 0;
	assert( ok );

	void* userparam = 0;
	HWND hwnd = CreateWindowExA( exstyle, classname, name, style, 0, 0, w, h, parenthwnd, 0, instance, userparam );
	if ( !hwnd )
	{
		UnregisterClass( classname, instance );
		return 0;
	}

	RECT cr;
	GetClientRect( hwnd, &cr );
	RECT wr;
	GetWindowRect( hwnd, &wr );
	MoveWindow( hwnd, wr.left, wr.top, w+(w-cr.right), h+(h-cr.bottom), TRUE );

	if ( fullscreen )
		ShowCursor( FALSE );
	return hwnd;
}

static bool flushMessages()
{
	MSG msg;
	if ( PeekMessage(&msg,0,0,0,PM_NOREMOVE) )
	{
		while ( WM_QUIT != msg.message &&
			PeekMessage(&msg,0,0,0,PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		return WM_QUIT != msg.message;
	}
	return true;
}

static void loadShaders( Context* context, const String& path, 
	bool recursesubdirs, ResourceManager* res )
{
	P(Shader) fx;
	for ( FindFile find(PathName(path,"*.fx").toString()) ; find.more() ; find.next() )
	{
		// ensure that shader basename is unique
		String shadername = find.data().path.toString();
		String basename = find.data().path.basename();
		int cloneix = s_shaderbasenames.indexOf( basename );
		if ( -1 != cloneix )
			throwError( Exception( Format("Shader basenames must be unique, but \"{0}\" and \"{1}\" have identical basenames", s_shaderfullnames[cloneix], shadername) ) );
		s_shaderfullnames.add( shadername );
		s_shaderbasenames.add( basename );

		fx = res->getShader( shadername, 0 );
		fx = res->getShader( shadername, 1 );
	}
}

static void loadParticles( Context* context, const String& path, 
	bool recursesubdirs, ResourceManager* res )
{
	P(ParticleSystem) prt;
	for ( FindFile find(PathName(path,"*.prt").toString()) ; find.more() ; find.next() )
	{
		// ensure that particle basename is unique
		String particlename = find.data().path.toString();
		String basename = find.data().path.basename();
		int cloneix = s_particlebasenames.indexOf( basename );
		if ( -1 != cloneix )
			throwError( Exception( Format("Particle basenames must be unique, but \"{0}\" and \"{1}\" have identical basenames", s_particlefullnames[cloneix], particlename) ) );
		s_particlefullnames.add( particlename );
		s_particlebasenames.add( basename );

		prt = res->getParticleSystem( find.data().path.toString(), find.data().path.parent().toString(), find.data().path.parent().toString() );
	}
}

static int run( HINSTANCE instance )
{
	try
	{
		char cwd[512];
		_getcwd( cwd, sizeof(cwd) );
		Debug::printf( "Current working directory is %s\n", cwd );

		_CrtSetDbgFlag( _CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) );

		assert( sizeof(s_keyToVk)/sizeof(s_keyToVk[0]) == App::KEY_COUNT && "App::KeyType codes don't match Win32 virtual key codes" );

		// application specific config
		App::Configuration config;
		config.name = "Example Application";
		config.width = 1024;
		config.height = 768;
		config.bits = 32;
		config.fullscreen = false;
		config.stencilbuffer = true;
		config.captureMouse = false;
		config.iconResourceID = 0;
		configure( config );
		s_captureMouse = config.captureMouse;
		#ifdef _DEBUG
		config.name = config.name + " (DEBUG build)";
		#endif
		
		// create main window
		s_hwnd = createMainWindow( __FILE__, "Loading...", config.width, config.height, config.fullscreen, instance, config.iconResourceID );
		if ( !s_hwnd )
		{
			MessageBox( 0, "Failed to create main window", "Error", MB_OK );
			return 1;
		}

		// init rendering context
		int surfaces = DX_Context::SURFACE_TARGET + DX_Context::SURFACE_DEPTH;
		if ( config.stencilbuffer )
			surfaces += DX_Context::SURFACE_STENCIL;
		s_context = new DX_Context( config.width, config.height, config.bits, 
			config.fullscreen ? DX_Context::WINDOW_FULLSCREEN : DX_Context::WINDOW_DESKTOP,
			DX_Context::RASTERIZER_HW,
			DX_Context::VERTEXP_HW,
			surfaces );

		// set default resource manager
		DefaultResourceManager::set( new DefaultResourceManager(s_context) );

		// try to load shader set first from data/shaders, then from KA3D/shaders
		String shaderpath = PathName( App::getDefaultDataPath(), "shaders").toString();
		if ( FindFile("data/shaders",0).more() )
			shaderpath = "data/shaders";
		::loadShaders( s_context, shaderpath, true, DefaultResourceManager::get(s_context) );

		// try to load particle set
		String particlepath = PathName( App::getDefaultDataPath(), "particles").toString();
		if ( FindFile("data/particles",0).more() )
			particlepath = "data/particles";
		::loadParticles( s_context, particlepath, true, DefaultResourceManager::get(s_context) );

		// init app instance
		App* app = init( s_context );
		assert( app != 0 );
		SetWindowText( s_hwnd, config.name.c_str() );

		// prepare for mouse input
		MOUSEMOVEPOINT prevmousepoint;
		ZeroMemory( &prevmousepoint, sizeof(prevmousepoint) );

		// main loop
		int oldtime = System::currentTimeMillis();
		while ( flushMessages() )
		{ 
			if ( config.captureMouse )
			{
				if ( s_active && s_context->ready() && !app->quitRequested() )
				{
					SetCapture( s_hwnd );
					ShowCursor( FALSE );

					RECT rc;
					GetClientRect( s_hwnd, &rc );
					POINT c;
					c.x = rc.right/2;
					c.y = rc.bottom/2;
					ClientToScreen( s_hwnd, &c );
					SetCursorPos( c.x, c.y );
				}
				else
				{
					ReleaseCapture();
					ShowCursor( TRUE );
				}
			}

			int time = System::currentTimeMillis();
			float dt = float(time-oldtime)*1e-3f;
			oldtime = time;

			if ( s_active && s_context->ready() || app->playingInput() )
			{
				if ( app->paused() )
					dt = 0.f;
				if ( app->recordingInput() || app->playingInput() )
					dt = 1.f / 30.f;
				if ( dt > 1.f )
					dt = 1.f;

				app->App::update( dt, s_context );
				app->update( dt, s_context );

				if ( app->quitRequested() )
				{
					ReleaseCapture();
					deinit();
					break;
				}
			}
		}
	}
	catch ( Throwable& e )
	{	
		App::deleteApp();
		hgr::Globals::cleanup();
		s_context = 0;

		// minimize window so that it doesn't overlap message box
		if ( s_hwnd != 0 )
		{
			ShowCursor( TRUE );
			MoveWindow( s_hwnd, 0, 0, 4, 4, TRUE );
		}

		MessageBox( s_hwnd, e.getMessage().format().c_str(), "Error", MB_OK );
	}

	if ( s_hwnd )
	{
		DestroyWindow( s_hwnd ); 
		s_hwnd = 0;
	}

	// make sure memory from static buffers are released
	{Array<String> tmp; s_shaderbasenames.swap( tmp );}
	{Array<String> tmp; s_shaderfullnames.swap( tmp );}
	{Array<String> tmp; s_particlebasenames.swap( tmp );}
	{Array<String> tmp; s_particlefullnames.swap( tmp );}
    return 0;
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE, LPSTR /*commandline*/, int /*commandshow*/ )
{
	int* allocmarker = new int(0x78563412);
	int retval = run( instance );
	lang::Globals::cleanup();
	delete allocmarker;
	_CrtDumpMemoryLeaks();
	return retval;
}

String App::getDefaultDataPath()
{
	// check if KA3D is defined, use that as default data path, otherwise data/
	const char* env = getenv("KA3D");
	if ( env == 0 )
		return "data/";
	else
		return PathName(env,"data").toString();
}

void App::loadShaders( const String& path, bool recursesubdirs )
{
	::loadShaders( s_context, path, recursesubdirs, DefaultResourceManager::get(s_context) );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
