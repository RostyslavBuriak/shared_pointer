#ifndef HEADER_S_PTR
#define HEADER_S_PTR

#include <atomic>

template <typename T>
class s_ptr{
private:
    struct te{ //type erasure idiom
        std::atomic<unsigned> counter;//for atomic thread-safe operations

        te():counter(1){};

        virtual void Delete() const = 0;

        virtual ~te(){};
    };

    template<typename Orig_Type,typename Deleter_Type>
    struct deleter : te{ //struct for deleter function handling
        Orig_Type * orig_pointer;
        Deleter_Type Deleter;

        deleter(Orig_Type * _p,Deleter_Type _d):
        orig_pointer(_p),
        Deleter(_d){}

        void Delete() const override{
            Deleter(orig_pointer);
        }
    };

    template<typename T1>
    struct def_del{ //functor for default delete
        void operator()(T1 * _p) const{
            delete _p;
        }
    };

    T * p; //pointer to the object
    te * count_del; //counter and deleter handler

    void incr(){ //atomic increment
        if(count_del)
            ++count_del->counter;
    }
    void decr(){ //atomic decrement
        if(count_del && !--count_del->counter) {  
            count_del->Delete(); 
            delete count_del; 
        }
    }
public:
    s_ptr():
    p(nullptr),
    count_del(nullptr)
    {}

    s_ptr(T * _p):
    p(_p),
    count_del(new deleter<T,def_del<T>>(p,def_del<T>())){}

    template<typename Deleter_Type>
    s_ptr(T * _p, Deleter_Type Deleter):
    p(_p),
    count_del(new deleter<T,Deleter_Type>(p,Deleter)){}

    s_ptr(const s_ptr& another_ptr):
    p(another_ptr.p),
    count_del(another_ptr.count_del){
        incr();
    }

    s_ptr(s_ptr&& another_ptr):
    p(another_ptr.p),
    count_del(another_ptr.count_del){}

    ~s_ptr(){
        decr();
    }

    s_ptr& operator=(const s_ptr& another_ptr){
        if(this != &another_ptr){
            decr();
            p = another_ptr.p;
            count_del = another_ptr.count_del;
            incr();
        }
        return *this;
    }

    s_ptr& operator=(s_ptr&& another_ptr){
        if(this != &another_ptr){
            decr();
            p = another_ptr.p;
            count_del = another_ptr.count_del;
        }
        return *this;
    }

    T* operator->(){
        return p;
    }

    T& operator*(){
        return *p;
    }
};

#endif