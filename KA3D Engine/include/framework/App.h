#ifndef _FRAMEWORK_APP_H
#define _FRAMEWORK_APP_H


#include <io/PathName.h>
#include <lang/Array.h>
#include <lang/Object.h>
#include <lang/String.h>


namespace io {
	class DataInputStream;
	class DataOutputStream;}

namespace gr {
	class Context;}


namespace framework
{


class FrameWindow;


/** 
 * Base class for application instances.
 * There must be only one application instance.
 * Application instance must always be allocated to heap (with operator new).
 * @ingroup framework
 */
class App
{
public:
	/** Initialization parameters. */
	struct Configuration
	{
		/** Name of the application (main window). */
		lang::String	name;
		/** Width of rendering target. */
		int				width;
		/** Height of rendering target. */
		int 			height;
		/** Bits per pixel. */
		int 			bits;
		/** Fullscreen window. */
		bool 			fullscreen;
		/** Stencil buffer required. */
		bool 			stencilbuffer;
		/** Capture all mouse input. */
		bool			captureMouse;
		/** Icon resource id. 0 if default. */
		int				iconResourceID;
	};

	/** Key type. */
	enum KeyType
	{
		///
		KEY_A,
		///
		KEY_B,
		///
		KEY_C,
		///
		KEY_D,
		///
		KEY_E,
		///
		KEY_F,
		///
		KEY_G,
		///
		KEY_H,
		///
		KEY_I,
		///
		KEY_J,
		///
		KEY_K,
		///
		KEY_L,
		///
		KEY_M,
		///
		KEY_N,
		///
		KEY_O,
		///
		KEY_P,
		///
		KEY_Q,
		///
		KEY_R,
		///
		KEY_S,
		///
		KEY_T,
		///
		KEY_U,
		///
		KEY_V,
		///
		KEY_W,
		///
		KEY_X,
		///
		KEY_Y,
		///
		KEY_Z,
		///
		KEY_1,
		///
		KEY_2,
		///
		KEY_3,
		///
		KEY_4,
		///
		KEY_5,
		///
		KEY_6,
		///
		KEY_7,
		///
		KEY_8,
		///
		KEY_9,
		///
		KEY_PAGEDOWN,
		///
		KEY_PAGEUP,
		///
		KEY_HOME,
		///
		KEY_END,
		///
		KEY_INSERT,
		///
		KEY_DELETE,
		///
		KEY_RETURN,
		///
		KEY_SHIFT,
		///
		KEY_CONTROL,
		///
		KEY_TAB,
		///
		KEY_SPACE,
		///
		KEY_BACKSPACE,
		///
		KEY_PAUSE,
		///
		KEY_ESCAPE,
		///
		KEY_F1,
		///
		KEY_F2,
		///
		KEY_F3,
		///
		KEY_F4,
		///
		KEY_F5,
		///
		KEY_F6,
		///
		KEY_F7,
		///
		KEY_F8,
		///
		KEY_F9,
		///
		KEY_F10,
		///
		KEY_F11,
		///
		KEY_F12,
		/** Arrow left. */
		KEY_LEFT,
		/** Arrow right. */
		KEY_RIGHT,
		/** Arrow up. */
		KEY_UP,
		/** Arrow down. */
		KEY_DOWN,
		/** Left mouse button. */
		KEY_LBUTTON,
		/** Middle mouse button. */
		KEY_MBUTTON,
		/** Right mouse button. */
		KEY_RBUTTON,
		KEY_COUNT
	};

	App();
	virtual ~App();

	/** 
	 * Main loop for your application.
	 * @param dt Time (seconds) since last update.
	 * @param context Rendering device context.
	 */
	virtual void		update( float dt, gr::Context* context );

	/**
	 * Called when user pressed key down.
	 * Default implementation updates state returned by isKeyDown and
	 * handles pause key so that update receives dt=0 if pause is enabled.
	 */
	virtual void		keyDown( KeyType key );

	/**
	 * Called when user releases key.
	 * Default implementation updates state returned by isKeyDown and
	 * handles pause key so that update receives dt=0 if pause is enabled.
	 */
	virtual void		keyUp( KeyType key );

	/**
	 * Called when user moves the mouse.
	 * If mouse has been captured (config.captureMouse=true in App init() function),
	 * then relative coordinates are received. Otherwise the coordinates
	 * are absolute, in window client area.
	 */
	virtual void		mouseMove( int dx, int dy );

	/**
	 * Called when main window is (de)activated.
	 */
	virtual void		activate( bool active );

	/**
	 * Loads all shaders from specific directory.
	 * After this the application can refer to the shader with
	 * its basename. For example after loading "high-end-shaders/bumpspecular.fx"
	 * application can simply request "bumpspecular" shader and
	 * get an instance of the previously loaded shader.
	 */
	void				loadShaders( const lang::String& path, bool recursesubdirs );

	/**
	 * Request application exit.
	 */
	void				quit();

	/**
	 * Sets default data path.
	 */
	void				setDataPath( const lang::String& datapath );

	/**
	 * Enables key-press and mouse input recording to specified file. Can be used to make
	 * simple repeats/recordings. Note: Recordings do NOT match
	 * exactly the original due to variable frame rate,
	 * so do not use this method in actual game to make a recording functionality.
	 */
	void				recordInput( const lang::String& filename );

	/**
	 * Playback of key-press and mouse input from specified file. Can be used to make
	 * simple repeats/recordings. Note: Recordings do NOT match
	 * exactly the original due to variable frame rate,
	 * so do not use this method in actual game to make a recording functionality.
	 */
	void				playInput( const lang::String& filename );

	/**
	 * Stops buffering keys and mouse movements.
	 */
	void				stopRecordingInput();

	/**
	 * Stops playing back buffered keys and mouse movements.
	 */
	void				stopPlayingInput();

	/**
	 * Returns true if application is recording key presses and mouse input.
	 * @see recordInput
	 */
	bool				recordingInput() const;

	/**
	 * Returns true if application is playing back key presses and mouse input.
	 * @see playInput
	 */
	bool				playingInput() const;

	/**
	 * Returns elapsed time in seconds.
	 */
	float				time() const;

	/**
	 * Returns expanded default path to data files.
	 */
	lang::String		path( const lang::String& filename="" ) const;

	/**
	 * Returns true if specified key is currently pressed down.
	 * State returned by this function is updated in default keyEvent implementation.
	 */
	bool				isKeyDown( KeyType key ) const;

	/**
	 * Returns true if application is paused.
	 */
	bool				paused() const			{return m_paused;}

	/**
	 * Returns true if application has requested exit.
	 */
	bool				quitRequested() const	{return m_quit;}

	/** 
	 * Returns the application instance. 
	 */
	static App*			get();

	/** 
	 * Deletes application instance. 
	 */
	static void			deleteApp();

	/**
	 * Converts key type to string representation.
	 */
	static const char*	toString( KeyType key );

	/**
	 * Returns true if App has been initialized.
	 */
	static bool			initialized();

	/**
	 * Returns path to default data directory.
	 */
	static lang::String	getDefaultDataPath();

private:
	io::PathName		m_dataPath;
	bool				m_keyDown[KEY_COUNT];
	bool				m_paused;
	bool				m_quit;

	/** Records key up/down transitions */
	struct KeyEvent
	{
		/** KeyType, MSB set if down */
		char	key;
		float	time;
	};

	/** Records mouse movement */
	struct MouseEvent
	{
		short	dx;
		short	dy;
		float	time;
	};

	float					m_time;
	lang::String			m_recordFilename;
	lang::Array<KeyEvent>	m_keyEvents;
	lang::Array<MouseEvent>	m_mouseEvents;

	App( const App& );
	App& operator=( const App& );
};


/**
 * Allows application to adjust rendering device initialization parameters.
 * Called before init().
 * Implemented by the application.
 */
void configure( App::Configuration& config );

/**
 * Initializes the application instance.
 * Implemented by the application.
 */
framework::App* init( gr::Context* context );


} // framework


#endif // _FRAMEWORK_APP_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
