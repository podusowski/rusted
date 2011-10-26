#ifndef COMMON_ICONTAINERNODE_HPP
#define COMMON_ICONTAINERNODE_HPP

#include <vector>

namespace Common
{
	template <class T>
	class IContainerNode
	{
	public:
		/**
		 * \brief Get children node.
		 * \param idx [in] index of node 
		 */
		virtual IContainerNode<T>* getChild(unsigned idx) const = 0;
	
		/**
		 * \brief Get parent node.
		 */
		virtual IContainerNode<T>* getParent() const = 0;

		/**
		 * \brief Get count of child nodes.
		 */
		virtual unsigned getChildrenCount() const = 0;
	
		/**
		 * \brief Get vector with objects in node.
		 */
		virtual std::vector<T> & getObjects() = 0;
	
		/**
		 * \brief Add object to node.
		 */
		virtual void addObject(T) = 0;
	
		/**
		 * \brief Remove object from node.
		 */
		virtual void removeObject(T) = 0;
	};
}

#endif
