#pragma once

#include "skse64/PapyrusArgs.h"

namespace VMResultArrayUtils
{
	/// <summary>
	/// Check if an item is in the specified list.
	/// </summary>
	/// <param name="theArray">The array to search in</param>
	/// <param name="value">The item to find</param>
	/// <returns>True if the item is found. False if not.</returns>
	template <typename T>
	bool HasItem(VMResultArray<T> theArray, T value)
	{
		if (std::find(theArray.begin(), theArray.end(), value) != theArray.end())
			return true;

		return false;
	}

	/// <summary>
	/// Find an item in a list and return the item index.
	/// </summary>
	/// <param name="theArray">The array to search in</param>
	/// <param name="value">The item to find</param>
	/// <returns>The item index. -1 if not found.</returns>
	template <typename T>
	int Find(VMResultArray<T> theArray, T value)
	{
		for (size_t i = 0; i < theArray.size(); i++)
		{
			if (theArray[i] == value)
				return i;
		}
		return -1;
	}

	/// <summary>
	/// Adds a list of items to the array.
	/// </summary>
	/// <param name="theArray">The array to add items in</param>
	/// <param name="newItems">The items to add</param>
	/// <param name="allowDuplicates">True if the list can have duplicates.</param>
	template <typename T>
	void Add(VMResultArray<T> theArray, VMResultArray<T> newItems, bool allowDuplicates = false)
	{
		for (size_t i = 0; i < newItems.size(); i++)
		{
			if (!allowDuplicates && HasItem(theArray, newItems[i]))
				continue;

			theArray.push_back(newItems[i]);
		}
	}
}