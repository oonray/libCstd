#include "ca_error.h"
#include <buffer/ca_buffer.hh>
#include <stdexcept>

ca::buffer::buffer(){
    this->opt=ca::buffer_option::NOEXPAND;
    this->mlen=ca::buffer_default::SIZE;
    this->dlen=0;
    this->data=(unsigned char *)std::calloc(1,this->mlen);
    if(this->data==NULL) throw std::bad_alloc();
}

ca::buffer::buffer(buffer_option opt){
    if(opt<0||opt>1) throw std::invalid_argument("Invalid option");
    this->opt=opt;
    this->mlen=ca::buffer_default::SIZE;
    this->dlen=0;
    this->data=(unsigned char *)std::calloc(1,this->mlen);
    if(this->data==NULL) throw std::bad_alloc();
}

ca::buffer::buffer(std::size_t size){
    if(size<=0) throw std::invalid_argument("did not provide a size");
    this->opt=ca::buffer_option::NOEXPAND;
    this->mlen=size;
    this->dlen=0;
    this->data=(unsigned char *)std::calloc(1,this->mlen);
    if(this->data==NULL) throw std::bad_alloc();
}

ca::buffer::buffer(std::size_t size,ca::buffer_option opt){
    if(size<=0) throw std::invalid_argument("did not provide a size");
    if(opt<0||opt>1) throw std::invalid_argument("Invalid option");
    this->opt=opt;
    this->mlen=size;
    this->dlen=0;
    this->data=(unsigned char *)std::calloc(1,this->mlen);
    if(this->data==NULL) throw std::bad_alloc();
}

ca::buffer::~buffer(){
    if(this->data!=NULL){
        memset(this->data,0,this->mlen);
        free(this->data);
        this->data=NULL;
    }
}

std::size_t ca::buffer::size() {return this->mlen;}
std::size_t ca::buffer::avaliable() {return this->dlen;}
std::size_t ca::buffer::left() {return this->mlen-this->dlen;}
int ca::buffer::full() {return this->dlen<this->mlen;}
int ca::buffer::expandable() {return this->opt;}
int ca::buffer::configure(buffer_option opt) {return this->opt;}

int ca::buffer::resize(std::size_t size){
    if(size<=0) throw std::invalid_argument("No provided size");

    unsigned char *n = (unsigned char *)realloc(this->data,this->mlen+size);
    ca_err_chk_null(n);

    this->data=n;
    this->mlen+=size;

    ca_err_return_bool();
}

int ca::buffer::write(void *in, size_t size){
    if(in==NULL) throw std::invalid_argument("Out is null");
    if(size<=0) throw std::invalid_argument("No provided size");
    if(size>this->mlen) throw std::runtime_error("Input to large");
    if(this->full()) throw std::runtime_error("No more space");
    if(this->left()<size){
        if(this->expandable()){
            this->resize(size);
        }else{
            throw std::runtime_error("No enough space");
        }
    }

    void * res = std::memcpy(this->data,in,size);
    if(res==NULL) throw std::runtime_error("could not copy");

    ca_err_return_bool()
}

int ca::buffer::read(void *out, size_t size){
    if(out==NULL) 
        throw std::invalid_argument("out is null");
    if(size<=0) 
        throw std::invalid_argument("did not provide a size");

    void *res = NULL;
    if(size<this->dlen){
        res = std::memcpy(out,this->data,this->dlen);
    }else{
        res = std::memcpy(out,this->data,size);
    }

    if(res==NULL) throw std::runtime_error("could not copy");

    ca_err_return_bool()
}
