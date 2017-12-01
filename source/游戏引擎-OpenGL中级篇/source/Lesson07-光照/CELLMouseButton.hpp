#pragma once

namespace CELL
{
    class  MouseButton
    {
    public:
        enum Enum
        {
            None = -1,

            Left = 0,
            Right,
            Middle,

            Button0 = 0,
            Button1,
            Button2,
            Button3,
            Button4,
            Button5,
            Button6,
            Button7,
            MAX,
            DWORD_FORCE =   0xFFFFFFFF,
        };

        MouseButton(Enum _value = None) :
        value(_value)
        {
        }

        friend bool operator == (MouseButton const& a, MouseButton const& b)
        {
            return a.value == b.value;
        }

        friend bool operator != (MouseButton const& a, MouseButton const& b)
        {
            return a.value != b.value;
        }

        int toValue() const
        {
            return (int)value;
        }
    private:
        Enum    value;
    };
}