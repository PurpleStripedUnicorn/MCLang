
#include "general/linkedlist.h"

template <typename T>
LinkedItem<T>::LinkedItem(T item) : item(item) {

}

template <typename T>
LinkedItem<T>::~LinkedItem() {

}

template <typename T>
T &LinkedItem<T>::getItem() const {
    return item;
}

template <typename T>
void LinkedItem<T>::setPrev(LinkedItem<T> *newPrev) {
    prev = newPrev;
}

template <typename T>
void LinkedItem<T>::setNext(LinkedItem<T> *newNext) {
    next = newNext;
}

template <typename T>
LinkedItem<T> *LinkedItem<T>::getPrev() const {
    return prev;
}

template <typename T>
LinkedItem<T> *LinkedItem<T>::getNext() const {
    return next;
}

template <typename T>
LinkedList<T>::LinkedList() : size(0), first(nullptr), last(nullptr) {

}

template <typename T>
LinkedList<T>::~LinkedList() {
    LinkedItem<T> *cur = first;
    while (cur != nullptr) {
        LinkedItem<T> *tmp = cur->getNext();
        delete cur;
        cur = tmp;
    }
}

template <typename T>
void LinkedList<T>::append(T item) {
    LinkedItem<T> *lItem = new LinkedItem<T>(item);
    if (last == nullptr) {
        first = lItem, last = lItem;
    } else {
        last->setNext(lItem);
        lItem->setPrev(last);
        last = lItem;
    }
    size++;
}

template <typename T>
void LinkedList<T>::prepend(T item) {
    LinkedItem<T> *lItem = new LinkedItem<T>(item);
    first->setPrev(lItem);
    lItem->setNext(first);
    if (first == nullptr) {
        first = lItem, last = lItem;
    } else {
        first->setPrev(lItem);
        lItem->setNext(first);
        first = lItem;
    }
    size++;
}

template <typename T>
void LinkedList<T>::remove(LinkedItem<T> *item) {
    LinkedItem<T> *iPrev = item->getPrev(), *iNext = item->getNext();
    delete item;
    if (iPrev == nullptr) {
        first = iNext;
        first->setPrev(nullptr);
    } else if (iNext == nullptr) {
        last = iPrev;
        last->setNext(nullptr);
    } else {
        iPrev->setNext(iNext);
        iNext->setPrev(iPrev);
    }
    size--;
}

template <typename T>
void LinkedList<T>::clear() {
    while (!empty())
        remove(first);
}

template <typename T>
inline bool LinkedList<T>::empty() const {
    return first == nullptr;
}

template <typename T>
void LinkedList<T>::insertBefore(LinkedItem<T> *ref, T item) {
    if (ref->getPrev() == nullptr) {
        prepend(item);
    } else {
        LinkedItem<T> *rPrev = ref->getPrev();
        LinkedItem<T> *lItem = new LinkedItem<T>(item);
        rPrev->setNext(lItem);
        ref->setPrev(lItem);
        lItem->setPrev(rPrev);
        lItem->setNext(ref);
        size++;
    }
}

template <typename T>
void LinkedList<T>::insertAfter(LinkedItem<T> *ref, T item) {
    if (ref->getNext() == nullptr) {
        append(item);
    } else {
        LinkedItem<T> *rNext = ref->getNext();
        LinkedItem<T> *lItem = new LinkedItem<T>(item);
        rNext->setPrev(lItem);
        ref->setNext(lItem);
        lItem->setPrev(ref);
        lItem->setNext(rNext);
        size++;
    }
}

template <typename T>
inline unsigned int LinkedList<T>::getSize() const {
    return size;
}