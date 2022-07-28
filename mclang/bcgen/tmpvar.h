
#ifndef __BCGEN_TMPVAR_H__
#define __BCGEN_TMPVAR_H__

#include <set>
#include <string>

/**
 * Keeps track of temporary variables, can be used to reserve and free these
 * variables when generating bytecode
 */
class TmpVarManager {

public:

    /**
     * Constructor
     */
    TmpVarManager();

    /**
     * Destructor
     */
    ~TmpVarManager();

    /**
     * Get the first temporary variable that is not reserved
     * @return A string with the name of the variable
     */
    std::string getFree() const;

    /**
     * Reserve the first free temporary variable
     * @return A string containing the name of the variable
     */
    std::string reserve();

    /**
     * Reserve a given temporary variable
     * @param name The temporary variable name
     * @note If the variable is already reserved, nothing changes
     */
    void reserve(std::string name);

    /**
     * Free the given variable
     * @param name The temporary variable name
     * @note If the variable is not reserved, nothing happens
     */
    void free(std::string name);

    /**
     * Get all of the reserved variables
     * @return A set containing all of the reserved variable names
     */
    std::set<std::string> getReserved() const;

    /**
     * Free all reserved variables
     */
    void clear();

private:

    /**
     * Reserve a given temporary variable given the index
     * @param index The index of the temporary variable
     * @note If the variable is already reserved, nothing changes
     */
    void reserve(unsigned int index);

    /**
     * Free the given variable given the index
     * @param index The temporary variable index
     * @note If the variable is not reserved, nothing happens
     */
    void free(unsigned int index);

    /**
     * Get the temporary variable name associated with the given index
     * @param index The variable index
     * @return A string containing the temporary variable name
     */
    std::string getVarName(unsigned int index) const;

    /**
     * Get the index from the given temporary variable name
     * @param name The temporary variable name
     * @return The index of the temporary variable
     * @warning Assumes that the name is a valid variable name!
     */
    unsigned int getVarIndex(std::string name) const;

    /**
     * Get the temporary variable prefix, each temporary variable starts with
     * this prefix
     * @return A string containing the prefix
     */
    std::string prefix() const;

    /**
     * Get the lowest index that is not reserved
     * @return The lowest positive number that is not in `reserved`
     */
    unsigned int firstFreeIndex() const;

    // List of reserved variables
    std::set<unsigned int> reserved;

};

#endif