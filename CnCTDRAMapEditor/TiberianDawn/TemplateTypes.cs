//
// Copyright 2020 Electronic Arts Inc.
//
// The Command & Conquer Map Editor and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// The Command & Conquer Map Editor and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection
using MobiusEditor.Model;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace MobiusEditor.TiberianDawn
{
    public static class TemplateTypes
    {
        public static readonly TemplateType Clear = new TemplateType(0, "clear1", 1, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate }, TemplateTypeFlag.Clear);
        public static readonly TemplateType Water = new TemplateType(1, "w1", 1, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Water2 = new TemplateType(2, "w2", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore1 = new TemplateType(3, "sh1", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore2 = new TemplateType(4, "sh2", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore3 = new TemplateType(5, "sh3", 1, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore4 = new TemplateType(6, "sh4", 2, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore5 = new TemplateType(7, "sh5", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore11 = new TemplateType(8, "sh11", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore12 = new TemplateType(9, "sh12", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore13 = new TemplateType(10, "sh13", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore14 = new TemplateType(11, "sh14", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore15 = new TemplateType(12, "sh15", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Slope1 = new TemplateType(13, "s01", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope2 = new TemplateType(14, "s02", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope3 = new TemplateType(15, "s03", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope4 = new TemplateType(16, "s04", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope5 = new TemplateType(17, "s05", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope6 = new TemplateType(18, "s06", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope7 = new TemplateType(19, "s07", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope8 = new TemplateType(20, "s08", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope9 = new TemplateType(21, "s09", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope10 = new TemplateType(22, "s10", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope11 = new TemplateType(23, "s11", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope12 = new TemplateType(24, "s12", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope13 = new TemplateType(25, "s13", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope14 = new TemplateType(26, "s14", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope15 = new TemplateType(27, "s15", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope16 = new TemplateType(28, "s16", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope17 = new TemplateType(29, "s17", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope18 = new TemplateType(30, "s18", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope19 = new TemplateType(31, "s19", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope20 = new TemplateType(32, "s20", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope21 = new TemplateType(33, "s21", 1, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope22 = new TemplateType(34, "s22", 2, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope23 = new TemplateType(35, "s23", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope24 = new TemplateType(36, "s24", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope25 = new TemplateType(37, "s25", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope26 = new TemplateType(38, "s26", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope27 = new TemplateType(39, "s27", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope28 = new TemplateType(40, "s28", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope29 = new TemplateType(41, "s29", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope30 = new TemplateType(42, "s30", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope31 = new TemplateType(43, "s31", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope32 = new TemplateType(44, "s32", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope33 = new TemplateType(45, "s33", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope34 = new TemplateType(46, "s34", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope35 = new TemplateType(47, "s35", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope36 = new TemplateType(48, "s36", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope37 = new TemplateType(49, "s37", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Slope38 = new TemplateType(50, "s38", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Shore32 = new TemplateType(51, "sh32", 3, 3, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly TemplateType Shore33 = new TemplateType(52, "sh33", 3, 3, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly TemplateType Shore20 = new TemplateType(53, "sh20", 4, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore21 = new TemplateType(54, "sh21", 3, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore22 = new TemplateType(55, "sh22", 6, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore23 = new TemplateType(56, "sh23", 2, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush1 = new TemplateType(57, "br1", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush2 = new TemplateType(58, "br2", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush3 = new TemplateType(59, "br3", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush4 = new TemplateType(60, "br4", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush5 = new TemplateType(61, "br5", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush6 = new TemplateType(62, "br6", 2, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush7 = new TemplateType(63, "br7", 2, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush8 = new TemplateType(64, "br8", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush9 = new TemplateType(65, "br9", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Brush10 = new TemplateType(66, "br10", 2, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Patch1 = new TemplateType(67, "p01", 1, 1, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Patch2 = new TemplateType(68, "p02", 1, 1, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Patch3 = new TemplateType(69, "p03", 1, 1, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Patch4 = new TemplateType(70, "p04", 1, 1, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Patch5 = new TemplateType(71, "p05", 2, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Patch6 = new TemplateType(72, "p06", 6, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Patch7 = new TemplateType(73, "p07", 4, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Patch8 = new TemplateType(74, "p08", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Shore16 = new TemplateType(75, "sh16", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore17 = new TemplateType(76, "sh17", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Shore18 = new TemplateType(77, "sh18", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Shore19 = new TemplateType(78, "sh19", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Patch13 = new TemplateType(79, "p13", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Patch14 = new TemplateType(80, "p14", 2, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Patch15 = new TemplateType(81, "p15", 4, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Boulder1 = new TemplateType(82, "b1", 1, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Boulder2 = new TemplateType(83, "b2", 2, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate, TheaterTypes.Desert });
        public static readonly TemplateType Boulder3 = new TemplateType(84, "b3", 3, 1, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Boulder4 = new TemplateType(85, "b4", 1, 1, new TheaterType[] { TheaterTypes.Temperate });
        public static readonly TemplateType Boulder5 = new TemplateType(86, "b5", 1, 1, new TheaterType[] { TheaterTypes.Temperate });
        public static readonly TemplateType Boulder6 = new TemplateType(87, "b6", 1, 1, new TheaterType[] { TheaterTypes.Temperate });
        public static readonly TemplateType Shore6 = new TemplateType(88, "sh6", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore7 = new TemplateType(89, "sh7", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore8 = new TemplateType(90, "sh8", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore9 = new TemplateType(91, "sh9", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Shore10 = new TemplateType(92, "sh10", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Road1 = new TemplateType(93, "d01", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road2 = new TemplateType(94, "d02", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road3 = new TemplateType(95, "d03", 1, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road4 = new TemplateType(96, "d04", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road5 = new TemplateType(97, "d05", 3, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road6 = new TemplateType(98, "d06", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road7 = new TemplateType(99, "d07", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road8 = new TemplateType(100, "d08", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road9 = new TemplateType(101, "d09", 4, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road10 = new TemplateType(102, "d10", 4, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road11 = new TemplateType(103, "d11", 2, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road12 = new TemplateType(104, "d12", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road13 = new TemplateType(105, "d13", 4, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road14 = new TemplateType(106, "d14", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road15 = new TemplateType(107, "d15", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road16 = new TemplateType(108, "d16", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road17 = new TemplateType(109, "d17", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road18 = new TemplateType(110, "d18", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road19 = new TemplateType(111, "d19", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road20 = new TemplateType(112, "d20", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road21 = new TemplateType(113, "d21", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road22 = new TemplateType(114, "d22", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road23 = new TemplateType(115, "d23", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road24 = new TemplateType(116, "d24", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road25 = new TemplateType(117, "d25", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road26 = new TemplateType(118, "d26", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road27 = new TemplateType(119, "d27", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road28 = new TemplateType(120, "d28", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road29 = new TemplateType(121, "d29", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road30 = new TemplateType(122, "d30", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road31 = new TemplateType(123, "d31", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road32 = new TemplateType(124, "d32", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road33 = new TemplateType(125, "d33", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road34 = new TemplateType(126, "d34", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road35 = new TemplateType(127, "d35", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road36 = new TemplateType(128, "d36", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road37 = new TemplateType(129, "d37", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road38 = new TemplateType(130, "d38", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road39 = new TemplateType(131, "d39", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road40 = new TemplateType(132, "d40", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road41 = new TemplateType(133, "d41", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road42 = new TemplateType(134, "d42", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Road43 = new TemplateType(135, "d43", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType River1 = new TemplateType(136, "rv01", 5, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River2 = new TemplateType(137, "rv02", 5, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River3 = new TemplateType(138, "rv03", 4, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River4 = new TemplateType(139, "rv04", 4, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River5 = new TemplateType(140, "rv05", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River6 = new TemplateType(141, "rv06", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River7 = new TemplateType(142, "rv07", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River8 = new TemplateType(143, "rv08", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River9 = new TemplateType(144, "rv09", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River10 = new TemplateType(145, "rv10", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River11 = new TemplateType(146, "rv11", 2, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River12 = new TemplateType(147, "rv12", 3, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River13 = new TemplateType(148, "rv13", 4, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType River14 = new TemplateType(149, "rv14", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River15 = new TemplateType(150, "rv15", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River16 = new TemplateType(151, "rv16", 6, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River17 = new TemplateType(152, "rv17", 6, 5, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River18 = new TemplateType(153, "rv18", 4, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River19 = new TemplateType(154, "rv19", 4, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River20 = new TemplateType(155, "rv20", 6, 8, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River21 = new TemplateType(156, "rv21", 5, 8, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River22 = new TemplateType(157, "rv22", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River23 = new TemplateType(158, "rv23", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River24 = new TemplateType(159, "rv24", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType River25 = new TemplateType(160, "rv25", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Ford1 = new TemplateType(161, "ford1", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Ford2 = new TemplateType(162, "ford2", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Falls1 = new TemplateType(163, "falls1", 3, 3, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Falls2 = new TemplateType(164, "falls2", 3, 2, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Desert, TheaterTypes.Temperate });
        public static readonly TemplateType Bridge1 = new TemplateType(165, "bridge1", 4, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Bridge1d = new TemplateType(166, "bridge1d", 4, 4, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Bridge2 = new TemplateType(167, "bridge2", 5, 5, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Bridge2d = new TemplateType(168, "bridge2d", 5, 5, new TheaterType[] { TheaterTypes.Winter, TheaterTypes.Temperate });
        public static readonly TemplateType Bridge3 = new TemplateType(169, "bridge3", 6, 5, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Bridge3d = new TemplateType(170, "bridge3d", 6, 5, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Bridge4 = new TemplateType(171, "bridge4", 6, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Bridge4d = new TemplateType(172, "bridge4d", 6, 4, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore24 = new TemplateType(173, "sh24", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore25 = new TemplateType(174, "sh25", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore26 = new TemplateType(175, "sh26", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore27 = new TemplateType(176, "sh27", 4, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore28 = new TemplateType(177, "sh28", 3, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore29 = new TemplateType(178, "sh29", 6, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore30 = new TemplateType(179, "sh30", 2, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore31 = new TemplateType(180, "sh31", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Patch16 = new TemplateType(181, "p16", 2, 2, new TheaterType[] { TheaterTypes.Winter });
        public static readonly TemplateType Patch17 = new TemplateType(182, "p17", 4, 2, new TheaterType[] { TheaterTypes.Winter });
        public static readonly TemplateType Patch18 = new TemplateType(183, "p18", 4, 3, new TheaterType[] { TheaterTypes.Winter });
        public static readonly TemplateType Patch19 = new TemplateType(184, "p19", 4, 3, new TheaterType[] { TheaterTypes.Winter });
        public static readonly TemplateType Patch20 = new TemplateType(185, "p20", 4, 3, new TheaterType[] { TheaterTypes.Winter });
        public static readonly TemplateType Shore34 = new TemplateType(186, "sh34", 3, 3, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly TemplateType Shore35 = new TemplateType(187, "sh35", 3, 3, new TheaterType[] { TheaterTypes.Temperate, TheaterTypes.Winter });
        public static readonly TemplateType Shore36 = new TemplateType(188, "sh36", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore37 = new TemplateType(189, "sh37", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore38 = new TemplateType(190, "sh38", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore39 = new TemplateType(191, "sh39", 1, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore40 = new TemplateType(192, "sh40", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore41 = new TemplateType(193, "sh41", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore42 = new TemplateType(194, "sh42", 1, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore43 = new TemplateType(195, "sh43", 1, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore44 = new TemplateType(196, "sh44", 1, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore45 = new TemplateType(197, "sh45", 1, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore46 = new TemplateType(198, "sh46", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore47 = new TemplateType(199, "sh47", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore48 = new TemplateType(200, "sh48", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore49 = new TemplateType(201, "sh49", 3, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore50 = new TemplateType(202, "sh50", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore51 = new TemplateType(203, "sh51", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore52 = new TemplateType(204, "sh52", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore53 = new TemplateType(205, "sh53", 4, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore54 = new TemplateType(206, "sh54", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore55 = new TemplateType(207, "sh55", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore56 = new TemplateType(208, "sh56", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore57 = new TemplateType(209, "sh57", 3, 2, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore58 = new TemplateType(210, "sh58", 2, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore59 = new TemplateType(211, "sh59", 2, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore60 = new TemplateType(212, "sh60", 2, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore61 = new TemplateType(213, "sh61", 2, 3, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore62 = new TemplateType(214, "sh62", 6, 1, new TheaterType[] { TheaterTypes.Desert });
        public static readonly TemplateType Shore63 = new TemplateType(215, "sh63", 4, 1, new TheaterType[] { TheaterTypes.Desert });

        private static TemplateType[] Types;

        static TemplateTypes()
        {
            Types =
                (from field in typeof(TemplateTypes).GetFields(BindingFlags.Static | BindingFlags.Public)
                 where field.IsInitOnly && typeof(TemplateType).IsAssignableFrom(field.FieldType)
                 select field.GetValue(null) as TemplateType).ToArray();
        }

        public static IEnumerable<TemplateType> GetTypes()
        {
            return Types;
        }
    }
}
