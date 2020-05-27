//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header: /CounterStrike/LINT.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LINT.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 03/28/96                                                     *
 *                                                                                             *
 *                  Last Update : March 28, 1996 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// overloading data member (e.g., IsToRedraw)
//lint -e1516

// function distinguishing error (actually not an error when conversion operators are used).
//lint -e1053


/*
**	The current version of Lint doesn't know how to deal with conversion operators. It
**	erroneously generates this error message as a result. Unfortunately, we have to throw
**	out the baby with the bathwater on this one.
*/
//lint -e58 -e56

/*
**	Hiding a non virtual member function is generally ok.
*/
//lint -e1511

/*
**	The warning about not initializing a member during construction is
**	valid. However, C&C takes advantage of this by using the in place
**	new operator during the load game process. Disable the warning, but
**	grudgingly.
*/
//lint -e1401

/*
**	Disable warning about non virtual base class destructor for these objects.
*/
//lint -esym(1509,GraphicBufferClass,GraphicViewPortClass,BufferClass,VideoViewPortClass,GetCDClass)
//lint -esym(1509,BasicTimerClass,FlyClass,FuseClass,StageClass,FlasherClass,CargoClass,DoorClass)
//lint -esym(1509,AbstractTypeClass)

/*
** The "unusual cast" warning is the result of derived casting. This is
** because there is no "dynamic_cast<>" implemented in Watcom (version 10.5).
*/
//lint -e740 -e571

/*
**	"function not previously declared inline" is probably not worth noting.
*/
//lint -e1727

/*
**	Allow sub-integer loss of precision. This will allow assigning ints to
**	chars without warning.
*/
//lint -e734

/*
**	Shifting an integer left is always ok.
*/
//lint -e701 -e703

/*
**	Allow repeated include files. The body of the include file should be
**	coded to ensure that it is processed only once.
*/
//lint -e537

/*
**	Implicitly converting an enum to an integer is almost always ok.
*/
//lint -e641

/*
**	It is possible to have template functions for "++" and "--" that
**	don't require one of the parameters to be a class object.
*/
//lint -e1042

/*
**	Redundant declarations are ok. They are a bit harder to maintain,
**	but they facilitate keeping modules less interdependant (include
**	file wise).
*/
//lint -e963 -e762 -e763

/*
**	Not having a default constructor is ok.
*/
//lint -e1712

/*
**	Private constructors are ok. In fact, they are necessary if the
**	class object must never be instantiated outside of the class
**	itself.
*/
//lint -e1704

/*
**	Ignoring the return value from a function is ok. It is very
**	common for certain side-effect type functions.
*/
//lint -e534

/*
**	Implicitly converting from a signed to an unsigned parameter (or
**	visa versa) is ok.
*/
//lint -e732 -e502 -e713 -e737 -eau

/*
**	Allow functions to overload and hide base functions. This is a
**	technique of inheritance that handles function parameter changes.
*/
//lint -e1411

/*
**	If a switch statement doesn't have a case for every value (enums)
**	but it does have a "default" case, then don't warn about it.
*/
//lint -e788

/*
**	If bitwize arithmetic is performed on compatible enumeration types,
**	then don't complain. Many enums are used in this fashion.
*/
//lint -e655 -e656

/*
**	If a data member is not explicitly initialized in the initializer
**	list, this is ok.
*/
//lint -e1542

/*
**	Calling "new" when not part of an assignment operation can be valid. This
**	is true if the "new" operator has been overloaded and the class keeps
**	track of itself.
*/
//lint -e522

/*
**	A class that is zero bytes long is ok. This is how method classes
**	usually work.
*/
//lint -e1501

/*
**	Boolean passed to function is ok.
*/
//lint -e730

/*
**	Signed/unsigned mix with relational... ignore for now.
*/
//lint -e574

