/*
http://mentorembedded.github.io/cxx-abi/abi.html#vtable
*/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
 
const int debug = 0;
 
void __pure_virtual_called() {
	fputs ("pure virtual function called\n", stderr);
	abort();
}
 
/* Translation of:
 
class A
{
public:
    virtual void OutA() = 0;
};
*/
 
struct A;
 
typedef struct  {
    void (*ptr__OutA) (struct A *__this);
} vtable__A;
 
typedef struct A {
    vtable__A *__vptr;
} A;
 
/* translation A::OutA() 
 * pure virtual function */
void A__OutA (A *__this) {
     __pure_virtual_called();
}
 
vtable__A vtable__A__A = { .ptr__OutA = A__OutA };
 
void A__constructor (A *__this) {
	if (debug)
		printf ("A__constructor %p\n", (void*)__this);
 
	/* dynamic type is initialised to A */
    __this->__vptr = &vtable__A__A;
}
 
/* Translation of:
 
class B
{
public:
    virtual void OutB() = 0;
};
 
*/
 
struct B;
 
typedef struct {
    void (*ptr__OutB)(struct B *__this);
} vtable__B;
 
typedef struct B {
    vtable__B *__vptr;
} B;
 
/* translation B::OutB() 
 * pure virtual function */
void B__OutB (B *__this) {
     __pure_virtual_called();
}
 
vtable__B vtable__B__B = { .ptr__OutB = B__OutB };
 
void B__constructor (B *__this) {
	if (debug)
		printf ("B__constructor %p\n", (void*)__this);
 
	/* dynamic type is initialised to B */
	__this->__vptr = &vtable__B__B;
}
/* Translation of:
 
class C : public A, public B
{
public:
    void OutA(); // overrides A::OutA()
    void OutB(); // overrides B::OutB()
    // note :
    // no new virtual function
};
 
*/
 
/* no new virtual function 
 * so no specific vtable type! */
 
typedef struct {
/* no additional vptr, we already have 2! */
    A base__A;
    B base__B;
} C;
 
/******* upcasts 
 * translation of 
 * static_cast<C*> (p) 
 */
 
/* translation of 
 * A *p;
 * static_cast<C*> (p);
 */
C *static_cast__A__C (A *__ptr) {
	/* 
	 * base__A is first member of C
	 * so offsetof(C, base__A) == 0
	 * can skip the pointer adjustment
	 */	
	return (C*)__ptr;
}
 
/* translation of 
 * B *p;
 * static_cast<C*> (p);
 */
C *static_cast__B__C (B *__ptr) {
	/* locate enclosing C object: 
	 * __base__B is not first member
	 * need to adjust pointer
	 */
	return (C*)((char*)__ptr - offsetof(C, base__B));
}
 
/* translation of virtual functions of C 
 * overriding function declarations from A
 */
 
/* translation of C::OutA() */
 
/* C::OutA() called from C */
void C__OutA (C *__this) {
    printf("Out A this=%p\n", (void*)__this);
}
 
/* C::OutA() called from A */
void C__A__OutA (A *__this) {
	if (debug)
			printf ("C__A__OutA %p\n", (void*)__this);
    C__OutA (static_cast__A__C (__this));
}
 
vtable__A vtable__A__C = { .ptr__OutA = C__A__OutA };
 
/* translation of virtual functions of C 
 * overriding function declarations from B
 */
 
/* translation of C::OutB() */
 
/* C::OutB() called from C */
void C__OutB (C *__this) {
    printf("Out B this=%p\n", (void*)__this);
}
 
/* C::OutB() called from B */
void C__B__OutB (B *__this) {
	if (debug)
			printf ("C__B__OutB %p\n", (void*)__this);
    C__OutB (static_cast__B__C (__this));
}
 
vtable__B vtable__B__C = { .ptr__OutB = C__B__OutB };
 
void C__constructor (C *__this) {
	if (debug)
		printf ("C__constructor %p\n", (void*)__this);
	/* construct subobjects */
    A__constructor (&__this->base__A);
    B__constructor (&__this->base__B);
 
	/* adjust dynamic type of this to C */
    __this->base__A.__vptr = &vtable__A__C;
    __this->base__B.__vptr = &vtable__B__C;
}
 
/* calls to C virtual functions with a C* 
 */
 
/* translation of 
 * C *p;
 * p->OutA();
 *
 * is
 * ((A*)p)->OutA();
 *
 * because C::OutA() is overrides A::OutA()
 */
void dyn__C__OutA (C *__this) {
	A *base_ptr__A = &__this->base__A;
	base_ptr__A->__vptr->ptr__OutA (base_ptr__A);
}
 
/* translation of 
 
int main()
{
    C obj;
    obj.OutA();
    obj.OutB();
 
    A *ap = &obj;
    B *bp = &obj;
	C *cp = &obj;
 
    ap->OutA();
    bp->OutB();
	cp->OutA();
 
    // Return
    return 0;
}
 
 *
 */
 
int main () {
	/* translation of:
	C obj; 
	*/
	C obj;
	C__constructor (&obj);
 
	/* translation of:
	obj.OutA();
	obj.OutB();
	 * obj is a locally declared object
	 * so dynamic type of obj is known as C
	 * can make direct call to C::OutA(), C::OutB()
	 */
	C__OutA (&obj);
	C__OutB (&obj);
 
	/* dumb (zero optimisation) translation of:
    A *ap = &obj;
    B *bp = &obj;
	C *cp = &obj;
	*/
	A *ap = &obj.base__A;
	B *bp = &obj.base__B;
	C *cp = &obj;
 
	/* translation of:
    ap->OutA();
    bp->OutB();
    cp->OutA();
 
	* dumb compiler = no optimisation
	* so dynamic type of *ap, *bp, *cp is unknown
	* so make "virtual" calls using vtable
	*/
	ap->__vptr->ptr__OutA(ap);
	bp->__vptr->ptr__OutB(bp);
	dyn__C__OutA (cp);
 
	/* note: obj lifetime ends now
	 * C has a trivial destructor 
	 * so no destructor call needed
	 */
 
	return 0;
}
