/* CircularList.h -- Circular List/Queue (Template)
 *
 * This material is a confidential trade secret and proprietary 
 * information of Tom Nelson D.B.A. Clockwork Software. which may not be 
 * reproduced, used, sold or transferred to any third party without the 
 * prior written consent of Tom Nelson.  This material is also copyrighted 
 * as an unpublished work under sections 104 and 408 of Title 17 of the 
 * United States Code.  Law prohibits unauthorized use, copying or reproduction.
 *
**/

// Revision History: 
//  5/10/00 Tom Nelson: Create file
// * Log at end-of-file *

// 100 columns
//34567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890


#ifndef _CircularList_T_h
#define _CircularList_T_h

template <class T>
class CircularList_T {
	typedef CircularList_T<T> Node;	// Short-hand
public:
	typedef T Member;

public:
	explicit CircularList_T(T *_pElement) : pElement(_pElement), pNext(this),pPrev(this) { }

	~CircularList_T() { Unlink(); }

	// Deletes all other nodes/elements in the list.
	// This assumes all nodes are not stacked or static.
	void DeleteElements() {
		Node *p;
		while ((p = NextNode()) != this)
			delete p->Element();
	}

	// *WARN* If the nodes are not part of the Element they will not be
	// unlinked after deleting the Element. If it is not unlinked we must
	// delete the node separately assuming the nodes have been allocated
	// individually. This is a bit touchy since we may be looking at freed memory.
	void PurgeList() {
		Node *p;
		while ((p = NextNode()) != this) {
			delete p->Element();
			if (!p->IsEmpty())
				delete p;
		}
	}
	// Link specified node after "this"
	Node * LinkHead(Node &link) { 
		link.Unlink();
		pNext->pPrev = &link;
		link.pNext = pNext;
		link.pPrev = this;
		pNext = &link;
		return &link;
	}
	Node * LinkHead(Node *pLink)	{ return LinkHead(*pLink); }

	// Link specified node before "this"
	// Same as LinkPrev if this is not the Terminal Node
	Node * LinkTail(Node &link)		{ return pPrev->LinkHead(link); }
	Node * LinkTail(Node *pLink)	{ return LinkTail(*pLink);		}

	// Insert specified list after "this"
	// Same as LinkNext if this is not the Terminal Node
	Node * InsertHead(Node &link) { 
		pNext->pPrev = link.pPrev;
		link.pPrev->pNext = pNext;
		link.pPrev = this;
		pNext = &link;
		return &link;
	}
	Node * InsertHead(T *pLink)		{ return InsertHead(*pLink); }

	// Insert specified list before "this"
	// Same as LinkPrev if this is not the Terminal Node
	Node * InsertTail(Node &link)	{ return pPrev->InsertHead(link); }
	Node * InsertTail(Node *pLink)	{ return InsertTail(*pLink);	  }

	// Unlink "this" from queue and return this.
	Node * Unlink() {
		pPrev->pNext = pNext;
		pNext->pPrev = pPrev;
		pPrev = pNext = this;
		
		return this;
	}
	BOOL IsLinked()		{ return pNext != this; }
	BOOL IsEmpty()		{ return !IsLinked();	}

	Node * NextNode() const	{ return pNext; }
	Node * PrevNode()	{ return pPrev; }
	
	T * Element()		{ return pElement;	}
	T * PrevElement()	{ return PrevNode()->Element(); }
	T * NextElement() const	{ return NextNode()->Element(); }

private:
	Node *pNext;
	Node *pPrev;
	T *pElement;
};


#endif	// _CircularList_T_h
