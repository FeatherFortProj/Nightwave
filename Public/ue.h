#pragma once
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

class FString {
public:
    using CharType = char16_t;
    inline static const size_t npos = -1;
    
private:
    CharType* data;
    int32_t length;
    int32_t capacity;
    
    static int32_t StrLen(const CharType* str) {
        if (!str) return 0;
        int32_t len = 0;
        while (str[len]) len++;
        return len;
    }
    
    void allocate(int32_t newCapacity) {
        if (newCapacity <= 0) {
            release();
            return;
        }
        
        CharType* newData = (CharType*)malloc(newCapacity * sizeof(CharType));
        if (newData) {
            if (data && length > 0) {
                int32_t copyLen = (length < newCapacity) ? length : newCapacity - 1;
                memcpy(newData, data, copyLen * sizeof(CharType));
                newData[copyLen] = 0;
                length = copyLen;
            } else {
                newData[0] = 0;
                length = 0;
            }
            
            free(data);
            data = newData;
            capacity = newCapacity;
        }
    }
    
public:
    FString() : data(nullptr), length(0), capacity(0) {}
    
    FString(const char* src) : data(nullptr), length(0), capacity(0) {
        if (src) {
            int32_t srcLen = strlen(src);
            if (srcLen > 0) {
                capacity = length = srcLen + 1;
                data = (CharType*)malloc(capacity * sizeof(CharType));
                if (data) {
                    for (int32_t i = 0; i < srcLen; i++) {
                        data[i] = (CharType)src[i];
                    }
                    data[srcLen] = 0;
                }
            }
        }
    }
    
    FString(const CharType* src) : data(nullptr), length(0), capacity(0) {
        if (src) {
            capacity = length = StrLen(src) + 1;
            data = (CharType*)malloc(capacity * sizeof(CharType));
            if (data) {
                memcpy(data, src, length * sizeof(CharType));
            }
        }
    }
    
    FString(const CharType* src, int32_t count) : data(nullptr), length(0), capacity(0) {
        if (src && count > 0) {
            capacity = length = count;
            data = (CharType*)malloc(capacity * sizeof(CharType));
            if (data) {
                memcpy(data, src, count * sizeof(CharType));
                data[count] = 0;
            }
        }
    }
    
    FString(int32_t size) : capacity(size + 1), length(size + 1) {
        data = (CharType*)malloc(capacity * sizeof(CharType));
        if (data) data[0] = 0;
    }
    
    FString(const FString& other) : data(nullptr), length(0), capacity(0) {
        if (other.data && other.length > 0) {
            capacity = length = other.length;
            data = (CharType*)malloc(capacity * sizeof(CharType));
            if (data) {
                memcpy(data, other.data, other.length * sizeof(CharType));
            }
        }
    }
    
    FString(FString&& other) noexcept : data(other.data), length(other.length), capacity(other.capacity) {
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }
    
    ~FString() {
        release();
    }
    
    FString& operator=(const FString& other) {
        if (this != &other) {
            release();
            if (other.data && other.length > 0) {
                capacity = length = other.length;
                data = (CharType*)malloc(capacity * sizeof(CharType));
                if (data) {
                    memcpy(data, other.data, other.length * sizeof(CharType));
                }
            }
        }
        return *this;
    }
    
    FString& operator=(FString&& other) noexcept {
        if (this != &other) {
            release();
            data = other.data;
            length = other.length;
            capacity = other.capacity;
            other.data = nullptr;
            other.length = 0;
            other.capacity = 0;
        }
        return *this;
    }
    
    void release() {
        if (data) {
            free(data);
            data = nullptr;
        }
        length = 0;
        capacity = 0;
    }
    
    void allocate() { data = (CharType*)malloc(capacity * sizeof(CharType)); }
    
    bool empty() const { return length == 0 || !data; }
    int32_t size() const { return length; }
    int32_t len() const { return length; }
    const CharType* c_str() const { return data ? data : (const CharType*)u""; }
    CharType* get() { return data; }
    const CharType* get() const { return data; }
    
    CharType& operator[](int32_t index) {
        return data[index];
    }
    
    const CharType& operator[](int32_t index) const {
        return data[index];
    }
    
    FString substr(int32_t offset, int32_t count = -1) {
        if (offset >= length) return FString();
        
        if (count == -1 || count > length - offset) 
            count = length - offset - 1;
            
        FString result(count);
        if (result.data && data && count > 0) {
            memcpy(result.data, data + offset, count * sizeof(CharType));
            result.data[count] = 0;
            result.length = count + 1;
        }
        return result;
    }
    
    int32_t find(CharType ch, int32_t start = 0) const {
        if (empty() || start < 0 || start >= length) return -1;
        
        for (int32_t i = start; i < length; i++) {
            if (data[i] == ch) return i;
        }
        return -1;
    }
    
    int32_t find(const CharType* pattern, int32_t start = 0) const {
        if (empty() || !pattern || start < 0 || start >= length) return -1;
        
        int32_t patternLen = StrLen(pattern);
        if (patternLen == 0 || length < patternLen) return -1;
        
        for (int32_t i = start; i <= length - patternLen; i++) {
            bool matched = true;
            for (int32_t j = 0; j < patternLen; j++) {
                if (data[i + j] != pattern[j]) {
                    matched = false;
                    break;
                }
            }
            if (matched) return i;
        }
        return -1;
    }
    
    bool ends_with(const CharType* suffix) const {
        if (!data || !suffix) return false;
        int32_t suffixLen = StrLen(suffix);
        if (suffixLen > length - 1) return false;
        
        int32_t startPos = (length - 1) - suffixLen;
        for (int32_t i = 0; i < suffixLen; i++) {
            if (data[startPos + i] != suffix[i]) return false;
        }
        return true;
    }
    
    bool starts_with(const CharType* prefix) const {
        if (empty() || !prefix) return false;
        int32_t prefixLen = StrLen(prefix);
        if (prefixLen > length) return false;
        
        for (int32_t i = 0; i < prefixLen; i++) {
            if (data[i] != prefix[i]) return false;
        }
        return true;
    }
    
    operator CharType*() { return data; }
    size_t find_first_of(CharType ch) { return find(ch); }
    
    std::wstring ToWString() const {
        std::wstring result;
        if (!empty()) {
            result.reserve(length);
            for (int32_t i = 0; i < length; i++) {
                result += static_cast<wchar_t>(data[i]);
            }
        }
        return result;
    }
    
    std::string ToString() const {
        std::string result;
        if (!empty()) {
            result.reserve(length);
            for (int32_t i = 0; i < length; i++) {
                if (data[i] <= 0x7F) result += (char)data[i];
                else result += '?';
            }
        }
        return result;
    }
    
    bool operator==(const FString& other) const {
        if (length != other.length) return false;
        if (empty() && other.empty()) return true;
        return memcmp(data, other.data, length * sizeof(CharType)) == 0;
    }
    
    bool operator!=(const FString& other) const {
        return !(*this == other);
    }
    
    explicit operator bool() const {
        return !empty();
    }
};