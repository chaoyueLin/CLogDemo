//
// Created by Charles on 2022/9/18.
//

#ifndef CLOGDEMO_AUTOBUFFER_H
#define CLOGDEMO_AUTOBUFFER_H


#include <sys/types.h>
#include <string.h>

class AutoBuffer {
public:
    enum TSeek {
        ESeekStart,
        ESeekCur,
        ESeekEnd,
    };

public:
    explicit AutoBuffer(size_t _size = 128);
    explicit AutoBuffer(void* _pbuffer, size_t _len, size_t _size = 128);
    explicit AutoBuffer(const void* _pbuffer, size_t _len, size_t _size = 128);
    ~AutoBuffer();

    void AllocWrite(size_t _readytowrite, bool _changelength = true);
    void AddCapacity(size_t _len);

    template<class T> void Write(const T& _val)
    { Write(&_val, sizeof(_val));}

    template<class T> void Write(off_t& _pos, const T& _val)
    { Write(_pos, &_val, sizeof(_val));}

    template<class T> void Write(const off_t& _pos, const T& _val)
    { Write(_pos, &_val, sizeof(_val));}

    void Write(const char* const _val)
    { Write(_val, strlen(_val));}

    void Write(off_t& _pos, const char* const _val)
    { Write(_pos, _val, strlen(_val));}

    void Write(const off_t& _pos, const char* const _val)
    { Write(_pos, _val, strlen(_val));}

    void Write(const AutoBuffer& _buffer);
    void Write(const void* _pbuffer, size_t _len);
    void Write(off_t& _pos, const AutoBuffer& _buffer);
    void Write(off_t& _pos, const void* _pbuffer, size_t _len);
    void Write(const off_t& _pos, const AutoBuffer& _buffer);
    void Write(const off_t& _pos, const void* _pbuffer, size_t _len);
    void Write(TSeek _seek, const void* _pbuffer, size_t _len);

    template<class T> size_t Read(T& _val)
    { return Read(&_val, sizeof(_val)); }

    template<class T> size_t Read(off_t& _pos, T& _val) const
    { return Read(_pos, &_val, sizeof(_val)); }

    template<class T> size_t Read(const off_t& _pos, T& _val) const
    { return Read(_pos, &_val, sizeof(_val)); }

    size_t Read(void* _pbuffer, size_t _len);
    size_t Read(AutoBuffer& _rhs , size_t _len);

    size_t Read(off_t& _pos, void* _pbuffer, size_t _len) const;
    size_t Read(off_t& _pos, AutoBuffer& _rhs, size_t _len) const;

    size_t Read(const off_t& _pos, void* _pbuffer, size_t _len) const;
    size_t Read(const off_t& _pos, AutoBuffer& _rhs, size_t _len) const;

    off_t Move(off_t _move_len);

    void Seek(off_t _offset,  TSeek _eorigin);
    void Length(off_t _pos, size_t _lenght);

    void* Ptr(off_t _offset=0);
    void* PosPtr();
    const void* Ptr(off_t _offset=0) const;
    const void* PosPtr() const;

    off_t Pos() const;
    size_t PosLength() const;
    size_t Length() const;
    size_t Capacity() const;

    void Attach(void* _pbuffer, size_t _len);
    void Attach(AutoBuffer& _rhs);
    void* Detach(size_t* _plen = NULL);

    void Reset();

private:
    void __FitSize(size_t _len);

private:
    AutoBuffer(const AutoBuffer& _rhs);
    AutoBuffer& operator = (const AutoBuffer& _rhs);

private:
    unsigned char* parray_;
    off_t pos_;
    size_t length_;
    size_t capacity_;
    size_t malloc_unitsize_;
};

extern const AutoBuffer KNullAtuoBuffer;

template <class S> class copy_wrapper_helper;

template <>
class copy_wrapper_helper<AutoBuffer> {
public:
    static void copy_constructor(AutoBuffer& _lhs, AutoBuffer& _rhs)
    { _lhs.Attach(_rhs); }

    static void copy_constructor(AutoBuffer& _lhs, const AutoBuffer& _rhs)
    { _lhs.Attach(const_cast<AutoBuffer&>(_rhs)); }

    static void destructor(AutoBuffer& _delobj) {}
};



#endif //CLOGDEMO_AUTOBUFFER_H
