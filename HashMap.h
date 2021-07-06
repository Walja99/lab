#ifndef HASHMAP_H
#define HASHMAP_H


#include <iostream>
#include <string>
#include <QString>
#include <fstream>
#include <cmath>

using namespace std;


unsigned hashFunction(QString name);

template <typename K, typename V>
class HashMap
{
  public:

    struct Item
    {
        Item(const K &key, const V &value);

        Item *_next;
        K _key;
        V _value;
    };

    class Iterator
    {
      public:
        Iterator(const HashMap &table);
        ~Iterator();

        void moveFwd();
        K getCurrentKey() const;
        V getCurrentValue() const;
        int getIndex() const {return _index;}

      private:
        Item *_item;
        int _index;
        const HashMap *_table;
    };

    HashMap();
    HashMap(const HashMap &table);
    ~HashMap();

    void add(const K &key, const V &value);

    bool isInMap(const K &key) const;

    void del(const K &key);
    void delAll();

    int count() const;

    void operator << (string fileName);
    void operator >> (string fileName) const;
    bool operator == (const HashMap &table) const;
    V operator [] (const K &key) const;

    void printData() const;

    K getFirst() const;
    K getLast() const;

  private:

    Item **_arrayPointer;
    int _itemNumber;
    int _tableSize;

    void resize();
};



template <typename K, typename V>
HashMap<K, V>::Item::Item(const K &key, const V &value)
{
    _key = key;
    _value = value;
    _next = NULL;
}


template <typename K, typename V>
HashMap<K, V>::HashMap()
{
    _tableSize = 32;
    _itemNumber = 0;
    _arrayPointer = new Item * [_tableSize];
    for (int i = 0; i < _tableSize; i++)
        _arrayPointer[i] = NULL;
}

template <typename K, typename V>
HashMap<K, V>::HashMap(const HashMap &table)
{
    _tableSize = table._tableSize;
    _arrayPointer = new Item* [_tableSize];
    for(int i = 0; i < _tableSize; i++)
    {
        if (table._arrayPointer[i] == NULL)
            _arrayPointer[i] = NULL;
        else
        {
            Item *otherItem = table._arrayPointer[i];
            Item *newItem = new Item(otherItem->_key, otherItem->_value);
            Item *previous = NULL;

            _arrayPointer[i] = newItem;
            otherItem = otherItem->_next;

            while (otherItem != NULL)
            {
                previous = newItem;
                newItem = new Item(otherItem->_key, otherItem->_value);
                previous->_next = newItem;
                otherItem = otherItem->_next;
            }
        }
    }
    _itemNumber = table._itemNumber;
}

template <typename K, typename V>
void HashMap<K, V>::add(const K &key, const V &value)
{
    unsigned index = hashFunction(key) % _tableSize;

    if (_arrayPointer[index] == NULL)
    {
        Item *newItem = new Item(key, value);
        _arrayPointer[index] = newItem;
        _itemNumber++;
    }
    else
    {
        Item *item = _arrayPointer[index];
        Item* tail = NULL;
        bool flag = false;

        while (item != NULL && flag == false)
        {
            if (item->_next == NULL)
                tail = item;
            if (item->_key == key)
            {
                item->_value += value;
                flag = true;
            }
            item = item->_next;
        }
        if (flag == false)
        {
            Item *newItem = new Item(key, value);
            tail->_next = newItem;
            _itemNumber++;
        }
    }
    if (_itemNumber > _tableSize)
        resize();
}

template <typename K, typename V>
void HashMap<K, V>::resize()
{
    Item **oldArrayPointer = _arrayPointer;
    int oldTableSize = _tableSize;

    _tableSize *= 2;
    _itemNumber = 0;
    _arrayPointer = new Item * [_tableSize];
    for (int i = 0; i < _tableSize; i++)
        _arrayPointer[i] = NULL;

    for (int i = 0; i < oldTableSize; i++)
        for (Item *pointer = oldArrayPointer[i]; pointer != NULL; pointer = pointer->_next)
            add(pointer->_key, pointer->_value);

    delete [] oldArrayPointer;
}

template <typename K, typename V>
bool HashMap<K, V>::isInMap(const K &key) const
{
    unsigned index = hashFunction(key) % _tableSize;
    Item *item = _arrayPointer[index];

    while(item != NULL)
    {
        if (item->_key == key)
            return true;
        item = item->_next;
    }
    return false;
}

template <typename K, typename V>
void HashMap<K, V>::del(const K &key)
{
    unsigned index = hashFunction(key) % _tableSize;
    Item *item = _arrayPointer[index];
    Item *previous = NULL;

    while(item != NULL && item->_key != key)
    {
        previous = item;
        item = item->_next;
    }

    if(item != NULL)
    {
        if (previous == NULL)
        {
            _arrayPointer[index] = item->_next;
            delete item;
        }
        else
        {
            previous->_next = item->_next;
            delete item;
        }
        _itemNumber--;
    }
}

template <typename K, typename V>
void HashMap<K, V>::delAll()
{
    Item *item = NULL, *itm = NULL;
    for (int i = 0; i < _tableSize; i++)
    {
        if(_arrayPointer[i] != NULL)
        {
            item = _arrayPointer[i];
            while(item != NULL)
            {
                itm = item;
                item = item->_next;
                delete itm;
            }
            _arrayPointer[i] = NULL;
        }
    }
    _itemNumber = 0;
}

template <typename K, typename V>
int HashMap<K, V>::count() const
{
    return _itemNumber;
}

template <typename K, typename V>
void HashMap<K, V>::operator >> (string fileName)  const
{
    ofstream fout(fileName.c_str());
    int cnt = 0;

    for (int i = 0; i < _tableSize; i++)
    {
        if(_arrayPointer[i] != NULL)
        {
            Item *item = _arrayPointer[i];

            while(item != NULL)
            {
                cnt++;

                fout << (item->_key).toStdString() << endl;
                fout << item->_value;
                if (cnt != _itemNumber)
                    fout << endl;

                item = item->_next;
            }
        }
    }
    fout.close();
}

template <typename K, typename V>
void HashMap<K, V>::operator << (string fileName)
{
    ifstream fin(fileName.c_str());

    char keyBuffer[81];
    char valueBuffer[81];

    delAll();

    while(!fin.eof())
    {
        fin.getline(keyBuffer, 81);
        fin.getline(valueBuffer, 81);

        add(QString(keyBuffer), atoi(valueBuffer));
    }
    fin.close();
}

template <typename K, typename V>
bool HashMap<K, V>::operator == (const HashMap &table)  const
{
    if(_tableSize != table._tableSize || _itemNumber != table._itemNumber)
        return false;

    for(int i = 0; i < _tableSize; i++)
    {
        if((_arrayPointer[i] == NULL && table._arrayPointer[i] != NULL) ||
           (_arrayPointer[i] != NULL && table._arrayPointer[i] == NULL))
            return false;

        if(_arrayPointer[i] != NULL && table._arrayPointer[i] != NULL)
        {
            Item *item = _arrayPointer[i], *otherItem = table._arrayPointer[i];

            while (item != NULL)
            {
               otherItem = table._arrayPointer[i];

               while(otherItem != NULL && (otherItem->_key != item->_key || otherItem->_value != item->_value))
                   otherItem = otherItem->_next;

               if(otherItem == NULL)
                   return false;

               item = item->_next;
            }
        }
    }
    return true;
}

template <typename K, typename V>
V HashMap<K, V>::operator [] (const K &key)  const
{
    unsigned index = hashFunction(key) % _tableSize;
    Item *item = _arrayPointer[index];

    while(item != NULL)
    {
        if (item->_key == key)
            return item->_value;
        item = item->_next;
    }
    return -1;
}

template <typename K, typename V>
HashMap<K, V>::~HashMap()
{
    delAll();
    delete [] _arrayPointer;
}

template <typename K, typename V>
HashMap<K, V>::Iterator::Iterator(const HashMap &table)
{
    int i = 0;
    _table = &table;

    while(i < table._tableSize && table._arrayPointer[i] == NULL)
        i++;
    if (i < table._tableSize)
    {
        _item = table._arrayPointer[i];
        _index = i;
    }
    else
    {
        _item = NULL;
        _index = -1;
    }
}

template <typename K, typename V>
HashMap<K, V>::Iterator::~Iterator()
{
}

template <typename K, typename V>
void HashMap<K, V>::Iterator::moveFwd()
{
    if(_item->_next != NULL)
        _item = _item->_next;
    else
    {
        int i = _index + 1;
        while(i < _table->_tableSize && _table->_arrayPointer[i] == NULL)
            i++;
        if(i < _table->_tableSize)
        {
            _item = _table->_arrayPointer[i];
            _index = i;
        }
        else
        {
            _item = NULL;
            _index = -1;
        }
    }
}

template <typename K, typename V>
K HashMap<K, V>::Iterator::getCurrentKey() const
{
    if(_item != NULL)
        return _item->_key;
    return "";
}

template <typename K, typename V>
V HashMap<K, V>::Iterator::getCurrentValue() const
{
    if(_item != NULL)
        return _item->_value;
    return -1;
}

template <typename K, typename V>
void HashMap<K, V>::printData() const
{
    Iterator *i = new Iterator(*this);
    while (i->getIndex() != -1)
    {
        cout << "Index: " << i->getIndex()
             << "  , Key: " << i->getCurrentKey()
             << "  , Value: " << i->getCurrentValue() << endl;
        i->moveFwd();
    }
}

template <typename K, typename V>
K HashMap<K, V>::getFirst() const
{
    Iterator *i = new Iterator(*this);

    if (i->_item == NULL)
        return "";
    return i->_item->_key;
}

template <typename K, typename V>
K HashMap<K, V>::getLast() const
{
    Iterator *i = new Iterator(*this);
    Iterator *j;

    if (i->_index == -1)
        return "";
    else
    {
        while (i->_index != -1)
        {
            j = i;
            i->moveFwd();
        }
        return j->_item->_key;
    }

}

#endif // HASHMAP_H
