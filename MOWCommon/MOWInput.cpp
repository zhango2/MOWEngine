#include "MOWGraphics\MOWCommon.h"
#include "MOWInput.h"

//---------------------------------------------
CMOWInput::CMOWInput()
{
    m_directInput = 0;
    m_keyboard = 0;
    m_mouse = 0;
    m_pressedKeys.resize(256);
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_mouseAcquired = false;
    m_keyboardAcquired = false;
    m_acquireMouse = false;
    m_acquireKeyboard = false;
}
//---------------------------------------------
CMOWInput::~CMOWInput()
{
    if(m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }

    // Release the keyboard.
    if(m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    // Release the main interface to direct input.
    if(m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }
}
//---------------------------------------------
void CMOWInput::Initialize( 
    HINSTANCE hinstance, 
    HWND hWnd, 
    int screenWidth, 
    int screenHeight 
    )
{
    HRESULT result = S_OK;
    ScreenDimension(screenWidth,screenHeight);

    result = DirectInput8Create(hinstance,0x0800,IID_IDirectInput8,(void**)&m_directInput,NULL);

    //Keyboard
    result = m_directInput->CreateDevice(GUID_SysKeyboard,&m_keyboard,NULL);
    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    result = m_keyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE);
    
    //Mouse
    result = m_directInput->CreateDevice(GUID_SysMouse,&m_mouse,NULL);
    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    result = m_mouse->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
    

}
//---------------------------------------------
void CMOWInput::Update()
{
    
    ReadKeyboard();    
    ReadMouse();
    ValidateMouseInput();

    if( m_mouseAcquired || m_keyboardAcquired )
    {
        NotifySinks();
    }
    
    
}
//---------------------------------------------
void CMOWInput::ScreenDimension( 
    int width, 
    int height 
    )
{
    m_screenWidth = width;
    m_screenHeight = height;
}
//---------------------------------------------
void CMOWInput::ReadKeyboard()
{
    HRESULT result;

    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(m_pressedKeys.size()*sizeof(char), (LPVOID)&m_pressedKeys[0]);
    if(FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED) && m_acquireKeyboard)
        {
            m_keyboard->Acquire();
        }
    }
    else
    {
        m_keyboardAcquired = true;
    }
}
//---------------------------------------------
void CMOWInput::ReadMouse()
{
    HRESULT result;

    // Read the mouse device.
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if(FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED) && m_acquireMouse)
        {
            m_mouse->Acquire();
        }
    }
    else
    {
        m_mouseAcquired = true;
    }
}
//---------------------------------------------
void CMOWInput::NotifySinks()
{
    
    if( m_mouseAcquired && m_keyboardAcquired )
    {
        for(IEvents* sink : m_sinks)
        {
            sink->KeyboardUpdated(m_pressedKeys);
            sink->MouseUpdated( m_mouseState.lX, m_mouseState.lY);
        }
    }
    
}
//---------------------------------------------
void CMOWInput::ValidateMouseInput()
{
    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    m_mouseX = m_mouseState.lX;
    m_mouseY = m_mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    m_mouseX = m_mouseX < 0 ? 0 : m_mouseX;
    m_mouseY = m_mouseY < 0 ? 0 : m_mouseY;
    m_mouseX = m_mouseX > m_screenWidth ? m_screenWidth : m_mouseX;
    m_mouseY = m_mouseY > m_screenHeight ? m_screenHeight : m_mouseY;

}
//---------------------------------------------
void CMOWInput::AddSink( 
    IEvents* sink 
    )
{
    m_sinks.push_back(sink);
}
//------------------------------------------------------
void CMOWInput::AcquireMouse(
    bool acquire
    )
{
    if( !acquire )
    {
        m_mouse->Unacquire();
        m_mouseAcquired = false;
    }
    else
    {
        m_mouse->Acquire();
    }
    m_acquireMouse = acquire;

    


}
//------------------------------------------------------
void CMOWInput::AcquireKeyboard(
    bool acquire
    )
{
    if( !acquire )
    {
        m_keyboard->Unacquire();
        m_keyboardAcquired = false;
    }
    else
    {
        m_keyboard->Acquire();
    }
    m_acquireKeyboard = acquire;
    
}

