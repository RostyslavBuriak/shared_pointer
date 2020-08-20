template <typename T>
class s_ptr{
public:
    s_ptr():
    p(nullptr),
    count_del()
    {}

    s_ptr(T * _p):
    p(_p),
    count_del(new deleter<T,void(&)()>(p,default_deleter))
    {}
    

private:
    struct te{ //type erasure idiom
        unsigned counter;
        te():counter(1){};

        virtual void Delete() = 0;

        virtual ~te(){};
    };

    template<typename Orig_Type,typename Deleter_Type>
    struct deleter : te{ //struct for deleter function handling
        OrigType * orig_pointer;
        Deleter_type deleter;

        te(OrigType * _p,Deleter_Type _d):
        orig_pointer(_p),
        deleter(_d){}

        void Delete() const override{
            deleter(orig_pointer);
        }
    };

    template<typename T>
    void default_deleter(const T * p){ //default delete function
        delete p;
    }
    

    T * p;
    te * count_del;
};