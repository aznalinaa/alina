#ifndef PATTERNTEMPLATES_H_INCLUDED
#define PATTERNTEMPLATES_H_INCLUDED
#ifndef PATTERNTEMPLATES_H
#define PATTERNTEMPLATES_H

// ��������� �����, ����������� ������� "��������"
template<typename T>
class Iterator {
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

// �������� ��� �������
template<typename T>
class ArrayIterator : public Iterator<T> {
private:
    T* ArrayContainer;
    size_t Pos;
    size_t Size;

public:
    ArrayIterator(T* container, size_t size)
        : ArrayContainer(container), Pos(0), Size(size) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return ArrayContainer[Pos]; }
};

// ��������� ����� "������"
template <typename T>
class ArrayClass {
private:
    static const size_t MaxSize = 100;
    T Items[MaxSize];
    size_t ArraySize;

public:
    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }
    T operator[](size_t index) const { return Items[index]; }
    T GetElement(size_t index) const { return Items[index]; }

    Iterator<T>* GetIterator() {
        return new ArrayIterator<T>(Items, ArraySize);
    }

    ArrayClass() : ArraySize(0) {}
};

// ��������� ��� ���������
template<typename T>
class IteratorDecorator : public Iterator<T> {
protected:
    Iterator<T>* It;

public:
    IteratorDecorator(Iterator<T>* it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

// ������� ��� ����������� STL
template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType> {
protected:
    ContainerType* Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType* container)
        : Container(container) {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};

#endif // PATTERNTEMPLATES_H


#endif // PATTERNTEMPLATES_H_INCLUDED
