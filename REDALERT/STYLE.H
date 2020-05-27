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

/*
       STYLE GUIDE FOR COMMAND & CONQUER : TIBERIAN SUN

In addition to the standard Westwood programmer's style guide, the following
guidelines apply.

> Use every feature the compiler has that will help catch bugs and encourage
solid portable coding practices. i.e., turn on all warnings and treat all warnings
as errors. Use helper programs such as Lint, BoundsChecker, or CodeGuard whenever
possible.

> Keep related items together. Examples: Declare variables right before they are
used. Keep functions that work on the same subsystem, within the same module.

> Use consistent commenting and spacing style. Examples: see existing Red Alert
code. Creative freedom does not extend to formatting.

> Use descriptive variable names. Examples: Use "index" rather than "i". Use
"unit_index" rather than "index".

> Use a consistent variable and function naming convention. Examples; boolean
variables should begin with "Is" (e.g., "IsActive", "IsFiring"). Functions that
exist solely to return a similar boolean query should begin with "Is_".

> If you have a variable that only serves a boolean function, then declare it
as "bool" rather than as "int" and assign it "true" or "false" rather than "1"
or "0".

> Choose function and variable names that are precise, descriptive, and concise (in that
order).

> Hide data where possible: Examples: If a global is only used in one module, make
it static to that module. If a helper function is only used in one module, make it
static as well. Hide class members in the private section if they aren't needed
public. Most variables aren't needed as public.

> Keep function bodies short. It is often times, more easy to understand and maintain
if a very long function is broken up into helper functions with descriptive names.
A clue if a function is too long is if you can page down 5 times and still be somewhere
in the middle of the function. If you can page down 10 times and still be in the
function, you've got a serious function size problem.

> Compare pointers to NULL and integer types to zero (NULL) instead of using the
"if (x)" or "if (!x)" format. This short format should only be used for boolean
values and expressions. It is just as efficient and it makes it more clear to the
reader that the variable is a pointer and not a simple boolean value.

> Use "const" when declaring any member functions that do not modify data. Corollary --
make sure any function that appears like it will not modify data will, in fact, NOT
modify any data. Example; A function called Is_Able_To_Move() shouldn't modify the
object nor should it actually cause something to start moving! The use of "const" is a
variation on style guide rule #1 (use the compiler to help ensure proper code).

> Shun using assembly language except in extreme cases. Such would be if a
function can only be performed by using assembly (requires special opcodes), or
if performance would be PROVEABLY dramatically improved. Don't code in assembly for
performance reasons if there isn't benchmarking code in place to analyze the results.
As a corollary, design a better algorithm as the first recourse.

> Keep class interfaces small and simple.

> Organize algorithms to use as few special case "if" statements as possible. Data
tables or data files are preferred. This follows the generally good guideline of offloading
as much processing as possible to compile-time rather than run-time.

*/
