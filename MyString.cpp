#include "MyString.h"

#include <cstring>
#include <utility>
#include <stdexcept>
#include <sstream>

MyString::MyString(const char* rawString) {
    if (rawString) {
        _data.save(rawString, strlen(rawString));
    }
}

MyString::MyString(const MyString& other) {
    if (other.size()) {
        _data.save(&other._data[0],
                   other.size());
    }
}

MyString::MyString(MyString&& other) noexcept {
    _data = std::move(other._data);
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        MyString copy(other);
        _data = std::move(copy._data);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this != &other) {
       _data = std::move(other._data);
    }
    return *this;
}

char& MyString::at(const unsigned int idx) {
    if (idx >= size()) {
        std::ostringstream str;
        str << "MyString::out of range error. Idx = "
            << idx
            << " >= this->size() which is "
            << size()
            << std::endl;
        throw std::out_of_range(str.str());
    }
    return operator[](idx);
}

const char& MyString::at(const unsigned int idx) const {
    if (idx >= size()) {
        std::ostringstream str;
        str << "MyString::out of range error. Idx = "
            << idx
            << " >= this->size() which is "
            << size()
            << std::endl;
        throw std::out_of_range(str.str());
    }
    return operator[](idx);
}

char& MyString::operator[](const unsigned int idx) {
    return _data[idx];
}

const char& MyString::operator[](const unsigned int idx) const {
    return _data[idx];
}

MyString& MyString::operator+(const MyString& appendedString) {
    this->append(appendedString);
    return *this;
}

void MyString::append(const MyString& appendedString) {
    this->insert(size(), appendedString);
}

MyString::~MyString() {
    _data.remove();
}

void MyString::insert(unsigned int pos, const MyString& insertedString) {
    if (pos > size()) {
        std::ostringstream str;
        str << "MyString::out of range error. Pos = "
            << pos
            << " >= this->size() which is "
            << size()
            << std::endl;
        throw std::out_of_range(str.str());
    }
    sso::string str(&_data[0], size() + insertedString.size());
    memcpy(&str[0] + pos, &insertedString._data[0], insertedString.size());
    memcpy(&str[0] + pos + insertedString.size(), &_data[0] + pos, size() - pos);
    _data = std::move(str);
}

void MyString::clear() {
    _data.remove();
}

void MyString::erase(unsigned int pos, unsigned int count) {
    if (pos > size()) {
        std::ostringstream str;
        str << "MyString::out of range error. Pos = "
            << pos
            << " >= this->size() which is "
            << size()
            << std::endl;
        throw std::out_of_range(str.str());
    }
    const unsigned int newSize = size() > count ? size() - count : pos;
    for (size_t i = pos; i < newSize; ++i) {
        (*this)[i] = (*this)[i + count];
    }
    sso::string str(&_data[0], newSize);
    _data = std::move(str);
}

unsigned int MyString::size() const {
    return _data.getSize();
}

bool MyString::isEmpty() const {
    return !size();
}

const char* MyString::rawString() const {
    char* str = new char[size() + 1];
    memcpy(str, &_data[0], size());
    str[size()] = '\0';
    return str;
}

unsigned int MyString::find(const MyString& substring, unsigned int pos) {
    if (this->isEmpty() || substring.isEmpty()) {
        return -1;
    }
    unsigned int res = -1;
    size_t count = 0;
    const size_t substrSize = substring.size();
    const size_t strSize = this->size();
    for (int i = strSize - (substrSize - 1) - 1; i >= (int)pos; --i) {
        for (int j = i; j < strSize; ++j) {
            if ((*this)[j] == substring[count]) {
                count++;
            }
            else {
                break;
            }
        }
        if (count == substrSize) {
            res = i;
            break;
        }
        count = 0;
    }
    return res;

}

int MyString::compare(const MyString& comparableString) const {
    const size_t minSize =  size() > comparableString.size()
                      ? comparableString.size()
                      : size();

    for (size_t i = 0; i < minSize; ++i) {
        if ((*this)[i] < comparableString[i]) {
            return -1;
        }
        else if ((*this)[i] > comparableString[i]) {
            return 1;
        }
    }
    return size() - comparableString.size();

}

bool MyString::operator!=(const MyString& comparableString) const {
    return !(*this == comparableString);
}

bool MyString::operator==(const MyString& comparableString) const {
    return this->compare(comparableString) == 0;
}

bool MyString::operator>(const MyString& comparableString) const {
    return this->compare(comparableString) > 0;
}

bool MyString::operator<(const MyString& comparableString) const {
    return this->compare(comparableString) < 0;
}

bool MyString::operator>=(const MyString& comparableString) const {
    return this->compare(comparableString) >= 0;
}

bool MyString::operator<=(const MyString& comparableString) const {
    return this->compare(comparableString) <= 0;
}
