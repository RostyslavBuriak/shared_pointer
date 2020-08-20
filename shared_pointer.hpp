#include <atomic>

template <typename T>
class s_ptr{
private:
    struct te{ //type erasure idiom
        std::atomic<unsigned> counter;//for atomic thread-safe operations

        te():counter(1){};

        virtual void Delete() = 0;

        virtual ~te(){};
    };

    template<typename Orig_Type,typename Deleter_Type>
    struct deleter : te{ //struct for deleter function handling
        Orig_Type * orig_pointer;
        Deleter_Type deleter;

        deleter(Orig_Type * _p,Deleter_Type _d):
        orig_pointer(_p),
        deleter(_d){}

        void Delete() const override{
            deleter(orig_pointer);
        }
    };

    template<typename T1>
    void default_deleter(const T1 * p){ //default delete function
        delete p;
    }
    

    T * p;
    te * count_del;
public:
    s_ptr():
    p(nullptr),
    count_del()
    {}

    s_ptr(T * _p):
    p(_p),
    count_del(new deleter<T,void(&)()>(p,default_deleter))
    {}

    template<typename Deleter_Type>
    s_ptr(T * _p, Deleter_Type Deleter):
    p(_p),
    count_del(new deleter<T,Deleter_Type>(p,Deleter))
    {}
    
};