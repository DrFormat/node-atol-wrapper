#include "utils.h"
//#include <stdio.h>

std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}

bool checkError(libfptr_handle fptr, int res, v8::Local<v8::Value>& error){
    if(res == 0) return false;
    int errorCode = libfptr_error_code(fptr);

    std::vector<wchar_t> errorDescription(32);
    int size = libfptr_error_description(fptr, &errorDescription[0], errorDescription.size());
    if (size > errorDescription.size())
    {
     errorDescription.resize(size);
     libfptr_error_description(fptr, &errorDescription[0], errorDescription.size());
    }
    std::string strDescr = ws2s(std::wstring(&errorDescription[0]));
    std::string errorString = "Ошибка - "+ std::to_string(errorCode) +" [ " + strDescr + " ]";
    v8::Local<v8::Object> errObj = Nan::Error(Nan::New(errorString).ToLocalChecked())->ToObject();
    Nan::Set(errObj, Nan::New("code").ToLocalChecked(), Nan::New(errorCode));
    Nan::Set(errObj, Nan::New("description").ToLocalChecked(), Nan::New(strDescr).ToLocalChecked());
    error = (v8::Local<v8::Value>)errObj;
    return true;
}

const std::wstring v8s2ws(v8::Local<v8::String> str) {
    Nan::Utf8String utf8str(str); // take the string arg and convert it to v8::string
    std::string sStr(*utf8str); // take the v8::string convert it to c++ class string
    return s2ws(sStr);
}
