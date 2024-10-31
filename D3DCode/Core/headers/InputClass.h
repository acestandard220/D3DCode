#ifndef _INPUT_CLASS_H_
#define _INPUT_CLASS_H_


class InputClass {
    public:
        InputClass();
        InputClass(const InputClass& i);
        ~InputClass();

    	bool Initialize();
        bool Shutdown();

        void KeyDown(unsigned int);
        void KeyUp(unsigned int);
        bool isKeyPressed(unsigned int);
    private:
        bool m_keys[256];
};
#endif // !_INPUT_CLASS_H_
