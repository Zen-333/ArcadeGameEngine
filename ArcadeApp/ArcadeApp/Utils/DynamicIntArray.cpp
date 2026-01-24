#include "DynamicIntArray.h"
#include <cassert>
#include <iostream>

using namespace std;

DynamicIntArray::DynamicIntArray(const DynamicIntArray& otherArray)
{
    bool result = Init(otherArray.mCapacity);
    assert(result);

    mSize = otherArray.mSize;

    for (size_t i = 0; i < mSize; ++i)
    {
        moptrData[i] = otherArray.moptrData[i];
    }
}

const DynamicIntArray& DynamicIntArray::operator=(const DynamicIntArray& otherArray)
{
    if (this == &otherArray)
    {
        return *this;
    }

    delete[] moptrData;

    bool result = Init(otherArray.mCapacity);
    assert(result);

    mSize = otherArray.mSize;

    for (size_t i = 0; i < mSize; ++i)
    {
        moptrData[i] = otherArray.moptrData[i];
    }

    return *this;
}

DynamicIntArray::~DynamicIntArray()
{
    delete[] moptrData;
    moptrData = nullptr;
}

bool DynamicIntArray::Init(size_t capacity)
{
    if (capacity == 0)
    {
        return false;
    }

    if (mCapacity == capacity)
    {
        return true;
    }

    if (moptrData)
    {
        delete[] moptrData;
        moptrData = nullptr;
    }

    moptrData = new (std::nothrow) int[capacity];
    if (!moptrData)
    {
        return false;
    }

    mSize = 0;
    mCapacity = capacity;
    return true;
}

bool DynamicIntArray::Reserve(size_t capacity)
{
    if (mCapacity < capacity)
    {
        //do the re-allocation
        int* oldData = moptrData;

        moptrData = new (std::nothrow) int[capacity];

        if (!moptrData)
        {
            moptrData = oldData;
            return false;
        }

        mCapacity = capacity;

        for (size_t i = 0; i < mSize; ++i)
        {
            moptrData[i] = oldData[i];
        }

        delete[] oldData;
    }

    return true;
}

bool DynamicIntArray::Resize(size_t newSize)
{
    if (mCapacity < newSize)
    {
        //need to reallocate
        bool reserveSucceeded = Reserve(newSize);
        if (!reserveSucceeded)
        {
            return reserveSucceeded;
        }
    }

    for (size_t i = mSize; i < newSize; ++i)
    {
        moptrData[i] = 0;
    }

    mSize = newSize;

    return true;
}

bool DynamicIntArray::PushBack(int newVal)
{
    assert(moptrData);
    if (!moptrData)
    {
        return false;
    }

    if (mSize + 1 > mCapacity)
    {
        bool reserveResult = Reserve(mCapacity * RESIZE_MULTIPLIER);

        if (!reserveResult)
        {
            return false;
        }
    }

    moptrData[mSize++] = newVal;

    return true;
}

bool DynamicIntArray::PopBack(int& value)
{
    assert(moptrData);
    if (!moptrData)
    {
        return false;
    }

    if (mSize > 0)
    {
        value = moptrData[--mSize];
        return true;
    }

    return false;
}

const int& DynamicIntArray::operator[](size_t index) const
{
    assert(moptrData && index < mSize);
    return moptrData[index];
}

int& DynamicIntArray::operator[](size_t index)
{
    assert(moptrData && index < mSize);
    return moptrData[index];
}

void DynamicIntArray::Display() const
{
    if (moptrData)
    {
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Capacity: " << mCapacity << std::endl;
        std::cout << "Size: " << mSize << std::endl;
        for (size_t i = 0; i < mSize; ++i)
        {
            std::cout << "val[" << i << "]: " << operator[](i) << std::endl;
        }

        std::cout << "---------------------------------------------" << std::endl;
    }
}

DynamicIntArray::DynamicIntArray(DynamicIntArray&& otherArray) :moptrData(otherArray.moptrData), mSize(otherArray.mSize), mCapacity(otherArray.mCapacity)
{
    otherArray.moptrData = nullptr;
    otherArray.mCapacity = 0;
    otherArray.mSize = 0;
}

DynamicIntArray& DynamicIntArray::operator=(DynamicIntArray&& otherArray)
{
    //make sure we're not assigning ourselves
    if (this == &otherArray)
    {
        return *this;
    }

    delete[] moptrData;

    moptrData = otherArray.moptrData;
    mSize = otherArray.mSize;
    mCapacity = otherArray.mSize;

    //make sure to invalidate otherArray
    otherArray.moptrData = nullptr;
    otherArray.mCapacity = 0;
    otherArray.mSize = 0;

    return *this;
}
