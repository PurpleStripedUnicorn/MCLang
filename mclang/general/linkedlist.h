
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

/**
 * Item in a doubly linked list
 * @tparam T The type of the item to store
 */
template<typename T>
class LinkedItem {

public:

    /**
     * Constructor
     * @param item The item to store
     */
    LinkedItem(T item);

    /**
     * Destructor
     */
    ~LinkedItem();

    /**
     * Get the actual item this node in the linked list represents
     * @return A reference to `item`
     */
    T &getItem() const;

    /**
     * Set the previous item pointer
     * @param newPrev The new previous item pointer
     */
    void setPrev(LinkedItem<T> *newPrev);

    /**
     * Set the next item pointer
     * @param newNext The new next item pointer
     */
    void setNext(LinkedItem<T> *newNext);

    /**
     * Get the previous item in the list
     * @return A pointer to the previous linked item, NULL if this is the first
     * item
     */
    LinkedItem<T> *getPrev() const;

    /**
     * Get the next item in the list
     * @return A pointer to the next linked item, NULL if this is the last item
     */
    LinkedItem<T> *getNext() const;

private:

    // The actual stored item
    T item;

    // Previous item in the list
    // NULL if this is the first item
    LinkedItem<T> *prev;

    // Next item in the list
    // NULL if this is the last item
    LinkedItem<T> *next;

};

/**
 * A basic doubly linked list with list operations
 * @tparam T The type of items to store in this linked list
 */
template<typename T>
class LinkedList {

public:

    /**
     * Constructor
     * @note By the default the linked list is empty on construction
     */
    LinkedList();

    /**
     * Destructor
     */
    ~LinkedList();

    /**
     * Append an item at the end of the linked list
     * @param item The item to append
     * @post Size is incremented
     */
    void append(T item);

    /**
     * Prepend an item to the start of the linked list
     * @param item The item to prepend
     * @post Size is incremented
     */
    void prepend(T item);

    /**
     * Remove an item from the list
     * @param item A pointer to the item
     * @post Size is decremented
     * @note Assumes that the pointer is a valid pointer to an item in the list
     */
    void remove(LinkedItem<T> *item);
    
    /**
     * Empty the list
     * @post Size is set to 0 and all linked items are deleted properly
     */
    void clear();

    /**
     * Check if the list is empty
     * @return A boolean which is true if the list is empty
     */
    inline bool empty() const;

    /**
     * Insert an item before another item
     * @param ref The reference item which the item should be put before
     * @param item The item to insert
     */
    void insertBefore(LinkedItem<T> *ref, T item);

    /**
     * Insert an item after another item
     * @param ref The reference item which the item should be put after
     * @param item The item to insert
     */
    void insertAfter(LinkedItem<T> *ref, T item);

    /**
     * Get the size of the list
     * @return The number of items in the linked list
     */
    inline unsigned int getSize() const;

private:

    // The current size of the linked list
    unsigned int size;

    // A pointer to the first item in the linked list
    // Will be NULL if the list is empty
    LinkedItem<T> *first;

    // A pointer to the last item in the linked list
    // Will be NULL if the list is empty
    LinkedItem<T> *last;

};

#endif