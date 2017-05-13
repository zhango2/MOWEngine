#ifndef MOWInput_H
#define MOWInput_H

#include <vector>
#include <dinput.h>

class CMOWInput
{
public:

    class IEvents
    {
    public:

        virtual void
            MouseUpdated(
                int x, int y
            ) = 0;

        virtual void
            KeyboardUpdated(
                const std::vector<char>& pressedKeys
            ) = 0;

    };

    CMOWInput(
    );

    ~CMOWInput(
    );



    void
        Initialize(
            HINSTANCE hinstance,
            HWND hWnd,
            int screenWidth,
            int screenHeight
        );

    void
        Update(
        );

    void
        ScreenDimension(
            int width,
            int height
        );

    void
        AddSink(
            IEvents* sink
        );

    void
        AcquireMouse(
            bool acquire
        );

    void
        AcquireKeyboard(
            bool acquire
            );

private:

    void                
        ReadKeyboard(
            );

    void                
        ReadMouse(
            );

    void                
        NotifySinks(
            );

    void                
        ValidateMouseInput(
            );

    std::vector<IEvents*>   
        m_sinks;

    IDirectInput8*          
        m_directInput;

    IDirectInputDevice8*    
        m_keyboard;

    IDirectInputDevice8*    
        m_mouse;

    std::vector<char>       
        m_pressedKeys;

    DIMOUSESTATE            
        m_mouseState;

    int                     
        m_screenWidth;

    int                     
        m_screenHeight;

    int                     
        m_mouseX;

    int                     
        m_mouseY;

    bool                    
        m_mouseAcquired;

    bool                    
        m_keyboardAcquired;

    bool
        m_acquireMouse;

    bool
        m_acquireKeyboard;
};

#endif