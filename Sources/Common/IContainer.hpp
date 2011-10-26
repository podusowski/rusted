#ifndef COMMON_ICONTAINER_H
#define COMMON_ICONTAINER_H

#include "IContainerNode.hpp"
#include "Point3.hpp"

namespace Common
{
	/**
	 * \brief Interface for containers
	 */
	template <class T> class IContainer
	{
	public:
		/**
		 * \brief Add object to tree at specified position.
		 */
		virtual void add(const T, const Common::Point3<Integer> &) = 0;

		/**
		 * \brief Get node from specified point.
		 */
		virtual IContainerNode<T> * getNodeAt(const Common::Point3<Integer> &) const = 0;
	};
}

#endif
