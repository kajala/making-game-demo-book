#include <framework/App.h>
#include <io/PathName.h>
#include <io/DataInputStream.h>
#include <io/FileInputStream.h>
#include <io/DataOutputStream.h>
#include <io/FileOutputStream.h>
#include <lang/Debug.h>
#include <config.h>


using namespace io;
using namespace lang;


namespace framework
{


static App* s_app = 0;

static const char* const s_keysz[] =
{
	"KEY_A",
	"KEY_B",
	"KEY_C",
	"KEY_D",
	"KEY_E",
	"KEY_F",
	"KEY_G",
	"KEY_H",
	"KEY_I",
	"KEY_J",
	"KEY_K",
	"KEY_L",
	"KEY_M",
	"KEY_N",
	"KEY_O",
	"KEY_P",
	"KEY_Q",
	"KEY_R",
	"KEY_S",
	"KEY_T",
	"KEY_U",
	"KEY_V",
	"KEY_W",
	"KEY_X",
	"KEY_Y",
	"KEY_Z",
	"KEY_1",
	"KEY_2",
	"KEY_3",
	"KEY_4",
	"KEY_5",
	"KEY_6",
	"KEY_7",
	"KEY_8",
	"KEY_9",
	"KEY_PAGEDOWN",
	"KEY_PAGEUP",
	"KEY_HOME",
	"KEY_END",
	"KEY_INSERT",
	"KEY_DELETE",
	"KEY_RETURN",
	"KEY_CONTROL",
	"KEY_SHIFT",
	"KEY_LCONTROL",
	"KEY_LSHIFT",
	"KEY_RCONTROL",
	"KEY_RSHIFT",
	"KEY_TAB",
	"KEY_SPACE",
	"KEY_BACKSPACE",
	"KEY_PAUSE",
	"KEY_ESCAPE",
	"KEY_F1",
	"KEY_F2",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_F10",
	"KEY_F11",
	"KEY_F12",
	"KEY_LEFT",
	"KEY_RIGHT",
	"KEY_UP",
	"KEY_DOWN",
	"KEY_LBUTTON",
	"KEY_MBUTTON",
	"KEY_RBUTTON",
};


App::App() :
	m_paused( false ),
	m_quit( false ),
	m_time( 0.f )
{
	assert( !s_app );
	s_app = this;

	for ( int i = 0 ; i < KEY_COUNT ; ++i )
		m_keyDown[i] = false;

	setDataPath( getDefaultDataPath() );
}

App::~App()
{
	try
	{
		stopRecordingInput();
	}
	catch ( ... )
	{
		Debug::printf( "ERROR: Failed to write \"%s\"", m_recordFilename.c_str() );
	}

	s_app = 0;
}

App* App::get()
{
	assert( s_app );
	return s_app;
}

void App::deleteApp()
{
	delete s_app;
	s_app = 0;
}

bool App::initialized()
{
	return s_app != 0;
}

void App::activate( bool /*active*/ )
{
	if ( !playingInput() )
	{
		for ( int i = 0 ; i < int(sizeof(m_keyDown)/sizeof(m_keyDown[0])) ; ++i )
			if ( m_keyDown[i] )
			{
				App::keyUp( (KeyType)i );
				keyUp( (KeyType)i );
			}
	}
}

void App::keyUp( KeyType key )
{
	m_keyDown[key] = false;

	if ( recordingInput() )
	{
		KeyEvent e;
		e.key = char(key);
		e.time = m_time;
		m_keyEvents.add( e );
	}
}

void App::keyDown( KeyType key )
{
	m_keyDown[key] = true;
	
	if ( key == KEY_PAUSE )
		m_paused = !m_paused;

	if ( recordingInput() )
	{
		KeyEvent e;
		e.key = char(key) | 0x80;
		e.time = m_time;
		m_keyEvents.add( e );
	}
}

void App::mouseMove( int dx, int dy )
{
	if ( recordingInput() )
	{
		MouseEvent e;
		e.dx = (short)dx;
		e.dy = (short)dy;
		e.time = m_time;
		m_mouseEvents.add( e );
	}
}

void App::quit()
{
	m_quit = true;
}

bool App::isKeyDown( KeyType key ) const
{
	return m_keyDown[key];
}

const char* App::toString( KeyType key )
{
	assert( sizeof(s_keysz)/sizeof(s_keysz[0]) == KEY_COUNT );
	return s_keysz[key];
}

void App::setDataPath( const String& datapath )
{
	m_dataPath = PathName(datapath);
}

String App::path( const String& filename ) const
{
	return PathName( m_dataPath.toString(), PathName(filename).toString() ).toString();
}

void App::stopRecordingInput()
{
	if ( recordingInput() )
	{
		FileOutputStream fileout( m_recordFilename );
		DataOutputStream dataout( &fileout );
		DataOutputStream* out = &dataout;
		
		int size = m_keyEvents.size();
		out->write( &size, sizeof(size) );
		out->write( &m_keyEvents[0], m_keyEvents.size()*sizeof(KeyEvent) );

		size = m_mouseEvents.size();
		out->write( &size, sizeof(size) );
		out->write( &m_mouseEvents[0], m_mouseEvents.size()*sizeof(MouseEvent) );

		m_recordFilename = "";
		m_time = 0.f;
	}
}

void App::stopPlayingInput()
{
	if ( playingInput() )
	{
		m_keyEvents.clear();
	}
}

void App::recordInput( const String& filename )
{
	assert( !recordingInput() );
	assert( !playingInput() );

	m_recordFilename = filename;
	m_time = 0.f;

	assert( recordingInput() );
}

void App::playInput( const String& filename )
{
	assert( !recordingInput() );
	assert( !playingInput() );

	m_recordFilename = "";
	m_time = 0.f;

	// load input data
	FileInputStream filein( filename );
	DataInputStream datain( &filein );
	DataInputStream* in = &datain;

	int size = 0;
	in->read( &size, sizeof(size) );
	m_keyEvents.resize( size );
	in->read( &m_keyEvents[0], m_keyEvents.size()*sizeof(KeyEvent) );

	size = 0;
	in->read( &size, sizeof(size) );
	m_mouseEvents.resize( size );
	in->read( &m_mouseEvents[0], m_mouseEvents.size()*sizeof(MouseEvent) );

	// terminating events (=never reached)
	KeyEvent ek;
	ek.key = 0;
	ek.time = 1e10f;
	m_keyEvents.add( ek );
	MouseEvent em;
	em.dx = 0;
	em.dy = 0;
	em.time = 1e10f;
	m_mouseEvents.add( em );

	assert( playingInput() );
}

void App::update( float dt, gr::Context* /*context*/ )
{
	if ( playingInput() )
	{
		Debug::printf( "playingInput( time=%g, dt=%g )\n", m_time, dt );

		// keys
		int count = 0;
		for ( ; count < m_keyEvents.size() ; ++count )
		{
			const KeyEvent& e = m_keyEvents[count];
			Debug::printf( "  key event %g %c\n", e.time, e.key );
			float t = e.time;
			if ( t > m_time+dt )
				break;

			KeyType key = (KeyType)(e.key&0x7F);
			if ( e.key & 0x80 )
			{
				keyDown( key );
				App::keyDown( key );
			}
			else
			{
				keyUp( key );
				App::keyUp( key );
			}
		}
		if ( count > 0 )
			m_keyEvents.remove( 0, count );

		// mouse movement
		count = 0;
		for ( ; count < m_mouseEvents.size() ; ++count )
		{
			const MouseEvent& e = m_mouseEvents[count];
			float t = e.time;
			if ( t > m_time+dt )
				break;

			mouseMove( e.dx, e.dy );
		}
		if ( count > 0 )
			m_mouseEvents.remove( 0, count );
	}

	m_time += dt;
}

bool App::recordingInput() const
{
	return m_recordFilename != "";
}

bool App::playingInput() const
{
	return m_recordFilename == "" && m_keyEvents.size() > 0;
}

float App::time() const
{
	return m_time;
}


} // framework

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
