#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
    class AutoRelease
    {
    public:
        AutoRelease() {}
        ~AutoRelease() {
            if(_pInstance) {
                delete _pInstance;
            }
        }
    };
public:
    template <typename... Args>
    static T * getInstance(Args... args)
    {
        if(_pInstance == nullptr) {
            _autoRelease;   // 露脸
            _pInstance = new T(args...);
        }
        return _pInstance;
    }
    static T * getInstance()
    {
        if(_pInstance == nullptr) {
            _autoRelease;   // 露脸
            _pInstance = new T();
        }
        return _pInstance;
    }

private:
    Singleton(){}
    ~Singleton() {}

private:
    static T * _pInstance;
    static AutoRelease _autoRelease;
};

template <typename T>
T * Singleton<T>::_pInstance = NULL;

template <typename T>
class Singleton<T>::AutoRelease Singleton<T>::_autoRelease;

#endif // SINGLETON_H
