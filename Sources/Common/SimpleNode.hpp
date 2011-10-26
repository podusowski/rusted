#ifndef COMMON_SIMPLENODE_HPP
#define COMMON_SIMPLENODE_HPP

#include "IContainerNode.hpp"

namespace Common
{
	template <class T>
	class SimpleNode : public IContainerNode<T>
	{
		std::vector<T>	objects;

	public:
		IContainerNode<T>* getChild(unsigned idx) const
		{
			return NULL;
		}

		IContainerNode<T>* getParent() const 
		{
			return NULL;
		}

		unsigned getChildrenCount() const
		{
			return 0;
		}

		std::vector<T> & getObjects() 
		{
			return objects;
		}

		void addObject(T object) 
		{
			objects.push_back (object);
		}

		void removeObject(T object)
		{
			typename std::vector<T>::iterator iter;

			for (iter = objects.begin(); iter != objects.end(); iter ++)
				if (*iter == object)
					objects.erase (iter);
		}
	};
}

#endif
