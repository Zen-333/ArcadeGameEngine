#pragma once

class DynamicIntArray {
public:
    DynamicIntArray() :moptrData(nullptr), mSize(0), mCapacity(0) {}
    //should clean up the memory
    ~DynamicIntArray();

    //copy
    DynamicIntArray(const DynamicIntArray& otherArray);
    const DynamicIntArray& operator=(const DynamicIntArray& otherArray);

    //move
    DynamicIntArray(DynamicIntArray&& otherArray);
    DynamicIntArray& operator=(DynamicIntArray&& otherArray);

    size_t Size() const { return mSize; }
    size_t Capacity() const { return mCapacity; }

    //Allocates the dynamic array
    bool Init(size_t capacity = INITIAL_CAPACITY);

    bool Reserve(size_t capacity);

    bool Resize(size_t newSize);

    bool PushBack(int newVal);
    bool PopBack(int& value);

    const int& operator[](size_t index) const;
    int& operator[](size_t index);
    void Display() const;

private:
    int* moptrData;
    size_t mSize;
    size_t mCapacity;

    static const size_t INITIAL_CAPACITY = 10;
    static const size_t RESIZE_MULTIPLIER = 2;
};
