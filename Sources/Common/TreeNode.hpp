#ifndef TREENODE_HPP
#define TREENODE_HPP

#include "IContainerNode.hpp"

namespace Common
{
	/**
	 * \brief Node of the tree.
	 */
	template <typename T> class TreeNode : public IContainerNode<T>
	{
		std::vector<T> 	objects;
		TreeNode<T> 	*children[8];
		TreeNode<T>		*parent;

	public:
		/**
		 * \brief Create node and children nodes.
		 * \param parent [in] parent node
		 * \param level [in] which level in tree
		 */
		TreeNode(TreeNode<T> *parent, unsigned level);
	
		/**
		 * \brief Free children.
		 */
		~TreeNode();
	
		/**
		 * \brief Get children node.
		 * \param idx [in] index of node 
		 */
		TreeNode<T>* getChild(unsigned idx) const;
	
		/**
		 * \brief Get parent node.
		 */
		TreeNode<T>* getParent() const;
	
		/**
		 * \brief Get vector with objects in node.
		 */
		std::vector<T> & getObjects();
	
		/**
		 * \brief Add object to node.
		 */
		void addObject(T);
	
		/**
		 * Remove object from node.
		 */
		void removeObject(T);

		unsigned getChildrenCount() const;
	};

	// implementation

	template <typename T> 
	TreeNode<T>::TreeNode(TreeNode<T> *parent, unsigned level)
			: parent (parent)
	{
		if (level)
		{
			for (int i = 0; i < 8; i ++)
				children [i] = new TreeNode<T>(this, level - 1);
		}
		else
		{
			for (int i = 0; i < 8; i ++)
				children [i] = NULL;
		}
	}

	template <typename T> 
	TreeNode<T>::~TreeNode()
	{
		for (int i = 0; i < 8; i ++)
			delete children[i];
	}

	template <typename T> 
	std::vector<T> & TreeNode<T>::getObjects()
	{
		return objects;
	}

	template <typename T> 
	TreeNode<T>* TreeNode<T>::getChild(unsigned idx) const
	{
		return children[idx];
	}

	template <typename T>
	TreeNode<T>* TreeNode<T>::getParent() const
	{
		return parent;
	}

	template <typename T>
	void TreeNode<T>::addObject(T object)
	{
		objects.push_back (object);
	}

	template <typename T>
	void TreeNode<T>::removeObject(T object)
	{
		typename std::vector<T>::iterator iter = objects.begin();
	
		for (; iter != objects.end(); iter ++)
			if (*iter == object)
				objects.erase(iter);
	}

	template <typename T>
	unsigned TreeNode<T>::getChildrenCount() const
	{
		return 8;
	}
}

#endif
