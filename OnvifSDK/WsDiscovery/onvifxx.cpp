#include <onvifxx.hpp>
#include <WebH.h>

struct Namespace namespaces[] = {};

std::string randUuid(soap * soap) {
    char rv[48];
    int r1, r2, r3, r4;
#ifdef WITH_OPENSSL
    r1 = soap_random;
    r2 = soap_random;
#else
    static int k = 0xFACEB00B;
    int lo = k % 127773;
    int hi = k / 127773;
# ifdef HAVE_GETTIMEOFDAY
    struct timeval tv;
    gettimeofday(&tv, NULL);
    r1 = 10000000 * tv.tv_sec + tv.tv_usec;
#else
    r1 = (int)time(NULL);
# endif
    k = 16807 * lo - 2836 * hi;
    if (k <= 0)
        k += 0x7FFFFFFF;
    r2 = k;
    k &= 0x8FFFFFFF;
    r2 += *(int*)soap->buf;
#endif
    r3 = soap_random;
    r4 = soap_random;
    sprintf(rv, "uuid:%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x",
            r1, (short)(r2 >> 16), (short)r2 >> 4,
            ((short)(r3 >> 16) & 0x3FFF) | 0x8000, (short)r3, r4);

    return rv;
}

Exception::Exception()
{
}

Exception::Exception(const char * msg) :
    msg_(msg)
{

}

Exception::~Exception() throw()
{
}

std::string & Exception::message()
{
    return msg_;
}

const char * Exception::what() const throw()
{
    return msg_.c_str();
}

SoapException::SoapException(soap * s) :
    code_(-1)
{
    if (s != NULL) {
        char buf[1024];
        soap_sprint_fault(s, buf, sizeof(buf));
        message().assign(buf);

        code_ = s->errnum;
    }
}

int SoapException::code() const
{
    return code_;
}

UnixException::UnixException(int code)
{
    if (code == 0)
        code = errno;

    char buf[1024];
    message().assign(strerror_r(code, buf, sizeof(buf) - 1));

    code_ = code;
}

int UnixException::code() const
{
    return code_;
}
