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

/* $Header: /CounterStrike/FINDPATH.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : FINDPATH.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : May 25, 1995   [PWG]                                         *
 *                                                                                             *
 * The path algorithm works by following a LOS path to the target. If it                       *
 * collides with an impassable spot, it uses an Edge following routine to                      *
 * get around it. The edge follower moves along the edge in a clockwise or                     *
 * counter clockwise fashion until finding the destination spot. The                           *
 * destination is determined by Find_Path. It is the first passable that                       *
 * can be reached (so it will handle the doughnut case, where there is                         *
 * a passable in the center of an unreachable area).                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Clear_Path_Overlap -- clears the path overlap list                                        *
 *   Find_Path -- Find a path from point a to point b.                                         *
 *   Find_Path_Cell -- Finds a given cell on a specified path                                  *
 *   Follow_Edge -- Follow an edge to get around an impassable spot.                           *
 *   FootClass::Unravel_Loop -- Unravels a loop in the movement path                           *
 *   Get_New_XY -- Get the new x,y based on current position and direction.                    *
 *   Optimize_Moves -- Optimize the move list.                                                 *
 *   Set_Path_Overlap -- Sets the overlap bit for given cell                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
//#include	<string.h>

/*
**	When an edge search is started, it can be performed CLOCKwise or
**	COUNTERCLOCKwise direction.
*/
#define	CLOCK				(FacingType)1	// Clockwise.
#define	COUNTERCLOCK	(FacingType)-1	// Counterclockwise.

/*
**	If defined, diagonal moves are allowed, else no diagonals.
*/
#define	DIAGONAL

/*
**	This is the marker to signify the end of the path list.
*/
#define	END			FACING_NONE

/*
**	"- 1" test for bit manipulation.
*/
#define TEST

/*
**	If memory is more important than speed, set this define to
**	true. It will then perform intermediate optimizations to get the most
**	milage out of a limited movement list staging area. If this value
**	is true then it figures paths a bit more intelligently.
*/
#define	SAVEMEM		true

/*
**	Modify this macro so that given two cell values, it will return
**	a value between 0 and 7, with 0 being North and moving
**	clockwise (just like map degrees).
*/
#define	CELL_FACING(a, b)		Dir_Facing(::Direction((a), (b)))


/*-------------------------------------------------------------------------*/
/*
**	Cells values are really indexes into the 'map'. The following value is
**	the X width of the map.
*/
#define	MODULO	MAP_CELL_W

/*
**	Maximum lookahead cells. Twice this value in bytes will be
**	reserved on the stack. The smaller this number, the faster the processing.
*/
#define MAX_MLIST_SIZE		300
#define THREAT_THRESHOLD	5


#define	MAX_PATH_EDGE_FOLLOW	400

#ifdef NEVER
typedef enum {
	FACING_N,			// North
	FACING_NE,			// North-East
	FACING_E,			// East
	FACING_SE,			// South-East
	FACING_S,			// South
	FACING_SW,			// South-West
	FACING_W,			// West
	FACING_NW,			// North-West

	FACING_COUNT			// Total of 8 directions (0..7).
} FacingType;
#endif


/*-------------------------------------------------------------------------*/
//static bool DrawPath;

inline FacingType Opposite(FacingType face)
{
	return( (FacingType) (face ^ 4));
}


inline static FacingType Next_Direction(FacingType facing, FacingType dir)
{
	facing = facing + dir;
	#ifndef DIAGONAL
		facing = (FacingType)(facing & 0x06);
	#endif
	return(facing);
}

/*=========================================================================*/
/* Define a couple of variables which are private to the module they are   */
/*      declared in.                                                       */
/*=========================================================================*/
static unsigned long MainOverlap[MAP_CELL_TOTAL/32];		// overlap list for the main path
static unsigned long LeftOverlap[MAP_CELL_TOTAL/32];		// overlap list for the left path
static unsigned long RightOverlap[MAP_CELL_TOTAL/32];	// overlap list for the right path


//static CELL MoveMask = 0;
static CELL DestLocation;
static CELL StartLocation;

/***************************************************************************
 * Point_Relative_To_Line -- Relation between a point and a line           *
 *                                                                         *
 *      If a point is on a line then the following function holds true:    *
 *      (x - x2)(z1 - z2) = (z - z2)(x1 - x2) given x,z a point on the     *
 *      line (x1,z1),(x2,z2).                                              *
 *      If the right side is > then the left side then the point is on one *
 *      side of the line and if the right side is < the the left side, then*
 *      the point is on the other side of the line.  By subtracting one side*
 *      from the other we can determine on what side (if any) the point is on*
 *      by testing the side of the resulting subtraction.                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      int   x    - x pos of point.                                       *
 *      int   z    - z pos of point.                                       *
 *      int   x1 - x pos of first end of line segment.                     *
 *      int   z1 - z pos of first end of line segment.                     *
 *      int   x1 - x pos of second end of line segment.                    *
 *      int   z1 - z pos of second end of line segment.                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *   Assuming (x1,z1) is north, (x2,z2) is south:                          *
 *       0 : point is on line.                                             *
 *       > 0 : point is east of line.                                      *
 *       < 0 : point is west of line.                                      *
 *                                                                         *
 * WARNINGS:                                                               *
 *    Remember that int means that assumes 16 bits of precision.           *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/28/1994 SKB : Created.                                             *
 *=========================================================================*/
int Point_Relative_To_Line(int x, int z, int x1, int z1, int x2, int z2)
{
	return((((long)x - (long)x2) * ((long)z1 - (long)z2)) - (((long)z - (long)z2) * ((long)x1 - (long)x2)));
}


/***************************************************************************
 * FootClass::Unravel_Loop -- Unravels a loop in the movement path         *
 *                                                                         *
 * While in the midst of the Follow Edge logic, it is possible (due to the *
 * fact that we support diagonal movement) to begin looping around a       *
 * column of some type.  The Unravel loop function will scan backward      *
 * through the list and fixup the path to try to prevent the loop.         *
 *                                                                         *
 * INPUT:      path   -   pointer to the generated path so we can pull the *
 *                         commands out of it.                             *
 *               cell   -   the cell we tried to enter that generated the  *
 *                        double overlap condition.                        *
 *               dir    -   the direction we tried to enter from when we   *
 *                        generated the double overlap condition           *
 *               startx -   the start x position of this path segment      *
 *               starty - the start y position of this path segment        *
 *               destx    - the dest x position for this path segment      *
 *               desty    - the dest y position for this path segment      *
 *                                                                         *
 * OUTPUT:      TRUE    - loop has been successfully unravelled             *
 *               FALSE  - loop can not be unravelled so abort follow edge  *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/25/1995 PWG : Created.                                             *
 *=========================================================================*/
bool FootClass::Unravel_Loop(PathType * path, CELL &cell, FacingType &dir, int sx, int sy, int dx, int dy, MoveType threshhold)
{
	/*
	** Walk back to the actual cell before we advanced our position
	*/
	FacingType	curr_dir	= dir;
	CELL			curr_pos = Adjacent_Cell(cell, Opposite(curr_dir));
	int			idx		= path->Length;			// start at the last position
	FacingType	* list		= &path->Command[idx-1];	// point to the last command
	int			checkx;
	int			checky;
	int			last_was_line	= false;

	/*
	** loop backward through the list searching for a point that is
	** on the line.  If the point was a diagonal move then adjust
	** it.
	*/
	while (idx) {
		checkx		= Cell_X(curr_pos);
		checky		= Cell_Y(curr_pos);

		if (!Point_Relative_To_Line(checkx, checky, sx, sy, dx, dy) || last_was_line) {

			/*
			** We have now found a point on the line.  Now we must check to see
			** if we left the line on a diagonal.  If we did then we need to fix
			** it up.
			*/
			if (curr_dir & 1 && curr_pos != path->LastFixup) {
				cell 				 = curr_pos;
				dir  				 = *(list-1);
				path->Length	 = idx;
				path->LastFixup = curr_pos;
				return(true);
			}

			last_was_line = !last_was_line;
		}

		/*
		** Since this cell will not be in the list, then pull out its cost
		*/
		path->Cost -= Passable_Cell(curr_pos, *list, -1, threshhold);

		/*
		** Remove this cells flag from the overlap list for the path
		*/
#ifdef TEST
		path->Overlap[curr_pos >> 5] &= ~(1 << ((curr_pos & 31)));
#else
		path->Overlap[curr_pos >> 5] &= ~(1 << ((curr_pos & 31) - 1));
#endif

		/*
		** Adjust to the next list position and direction.
		*/
		curr_dir = *list--;
		curr_pos	= Adjacent_Cell(curr_pos, Opposite(curr_dir));
		idx--;
	}

	/*
	** If we can't modify the list to eliminate the problem, then we have
	** a larger problem in that we have deleted all of the cells in the
	** list.
	*/
	return(false);
}


/***************************************************************************
 * Register_Cell -- registers a cell on our path and check for backtrack   *
 *                                                                         *
 * This function adds a new cell to our path.  If the cell has already     *
 * been recorded as part of our path, then this function moves back down   *
 * the list truncating it at the point we registered that cell.  This      *
 * function will eliminate all backtracking from the list.                 *
 *                                                                         *
 * INPUT:      long   * list - the list to set the overlap bit for         *
 *               CELL  cell    - the cell to mark on the overlap list      *
 *                                                                         *
 * OUTPUT:     BOOL - TRUE if bit has been set, FALSE if bit already set   *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/23/1995 PWG : Created.                                             *
 *=========================================================================*/
bool FootClass::Register_Cell(PathType * path, CELL cell, FacingType dir, int cost, MoveType threshhold)
{
	FacingType  * list;
	int 	pos  = cell >> 5;
#ifdef TEST
	int	bit  = (cell & 31);
#else
	int	bit  = (cell & 31) - 1;
#endif

	/*
	** See if this point has already been registered as on the list.  If so
	** we need to truncate the list back to this point and register the
	** new direction.
	*/
	if (path->Overlap[pos] & (1 << bit)) {
		/*
		** If this is not a case of immediate back tracking then handle
		** by searching the list to see what we find.  However is this is
		** an immediate back track, then pop of the last direction
		** and unflag the cell we are in (not the cell we are moving to).
		** Note: That we do not check for a zero length cell because we
		** could not have a duplicate unless there are cells in the list.
		*/

		if (path->Command[path->Length - 1] == Opposite(dir)) {
			CELL pos = Adjacent_Cell(cell, Opposite(dir));
#ifdef TEST
			path->Overlap[pos >> 5] &= ~(1 << ((pos & 31)));
#else
			path->Overlap[pos >> 5] &= ~(1 << ((pos & 31) - 1));
#endif
			path->Length--;
		} else {
			/*
			** If this overlap is in the same place as we had our last overlap
			** then we are in a loop condition.  We need to signify that we
			** cannot register this cell.
			*/
			if (path->LastOverlap == cell) {
				return(false);
			} else {
				path->LastOverlap = cell;
			}

			CELL pos 	  	= path->Start;
			int newlen		= 0;
			int idx 		   = 0;
			list		      = path->Command;

			/*
			** Note that the cell has to be in this list, so theres no sense
			** in checking whether we found it (famous last words).
			**
			** PWG 8/16/95 - However there is no sense searching the list if
			**               the cell we have overlapped on is the cell we
			**               started in.
			*/

			if (pos != cell) {
				while (idx < path->Length) {
					pos = Adjacent_Cell(pos, *list);
			  		if (pos == cell) {
						idx++;
						list++;
						break;
					}
					idx++;
					list++;
				}
				newlen = idx;
			}

			/*
			** Now we are pointing at the next command in the list.  From here on
			** out we need to unmark the fact that we have entered these cells and
			** adjust the cost of our path to reflect that we have not entered
			** then.
			*/
			while (idx < path->Length) {
				pos			= Adjacent_Cell(pos, *list);
				path->Cost -= Passable_Cell(pos, *list, -1, threshhold);
#ifdef TEST
				path->Overlap[pos >> 5] &= ~(1 << ((pos & 31)));
#else
				path->Overlap[pos >> 5] &= ~(1 << ((pos & 31) - 1));
#endif
				idx++;
				list++;
			}
			path->Length = newlen;
		}
	} else {
		/*
		** Now we need to register the new direction, updating the cell structure
		** and the cost.
		*/
		int cpos 				= path->Length++;
		path->Command[cpos]	= dir;			// save of the direction we moved
		path->Cost 			  += cost;			// figure new cost for cell
		path->Overlap[pos]  |= (1 << bit);	// mark the we have entered point
	}
	return(true);
}


/***********************************************************************************************
 * Find_Path -- Find a path from point a to point b.                                           *
 *                                                                                             *
 * INPUT:      int source x,y, int destination x,y, char *final moves                          *
 *             array to store moves, int maximum moves we may attempt                          *
 *                                                                                             *
 * OUTPUT:     int number of moves it took (IMPOSSIBLE_MOVES if we could                       *
 *             not reach the destination                                                       *
 *                                                                                             *
 * WARNINGS:   This algorithm assumes that the target is NOT situated                          *
 *             inside an impassable. If this case may arise, the do-while                      *
 *             statement inside the inner while (true) must be changed                         *
 *             to include a check to se if the next_x,y is equal to the                        *
 *             dest_x,y. If it is, then return(IMPOSSIBLE_MOVES).                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1991  CY : Created.                                                                 *
 *=============================================================================================*/
PathType * FootClass::Find_Path(CELL dest, FacingType * final_moves, int maxlen, MoveType threshhold)
{
	CELL					source = Coord_Cell(Coord);		// Source expressed as cell
	static PathType	path;										// Main path control.
	CELL					next;										// Next cell to enter
	CELL					startcell;								// Cell we started in
	FacingType			direction;								// Working direction of look ahead.
	FacingType			newdir;									// Tentative facing value.

	bool					left=false, 							// Was leftward path legal?
							right=false;							// Was rightward path legal?

	int					len;										// Length of detour command list.
	int					unit_threat;							// Calculated unit threat rating
	int					cost;										// Cost to enter the square
	FacingType			moves_left[MAX_MLIST_SIZE+2], 	// Counterclockwise move list.
							moves_right[MAX_MLIST_SIZE+2];	// Clockwise move list.
	PathType				pleft,pright;							// Path control structures.
	PathType *			which;									// Which path to actually use.
	int					threat = 0;			//
	int					threat_stage = 0; //These weren't initialized. ST - 1/8/2019 12:03PM


	/*
	** If we have been provided an illegal place to store our final moves
	** then forget it.
	*/
	if (!final_moves) return(NULL);

	BStart(BENCH_FINDPATH);

	PathCount++;

	if (Team && Team->Class->IsRoundAbout) {
		unit_threat			= (Team) ? Team->Risk : Risk();
		threat_stage		= 0;
		threat				= 0;
	} else {
		unit_threat = threat = -1;
	}

	StartLocation = source;
	DestLocation = dest;

	/*
	** Initialize the path structure so that we can keep track of the
	** path.
	*/
	path.Start			= source;
	path.Cost			= 0;
	path.Length 		= 0;
	path.Command 		= final_moves;
	path.Command[0] 	= END;
	path.Overlap		= MainOverlap;
	path.LastOverlap	= -1;
	path.LastFixup		= -1;

	memset(path.Overlap, 0, sizeof(MainOverlap));

	/*
	** Clear the over lap list and then make sure that our starting position is marked
	** on the overlap list.  (Otherwise the harvesters will drive in circles... )
	*/
#ifdef TEST
	path.Overlap[source >> 5] |= (1 << ((source & 31)));
#else
	path.Overlap[source >> 5] |= (1 << ((source & 31) - 1));
#endif

	startcell 			= source;

	/*
	**	Account for trailing end of list command, so reduce the maximum
	**	allowed legal commands to reflect this.
	*/
	maxlen--;

	/*
	**	As long as there is room to put commands in the movement command list,
	** then put commands in it.  We build the path using the following
	** methodology.
	**
	** 1. Scan through the desired straight line path until we either hit an
	**    impassable or have created a valid path.
	**
	** 2. If we have hit an impassable, walk through the impassable to make
	**    sure that there is a passable on the other side.  If there is not
	**    and we can not change the impassable, then this list is dead.
	**
	** 3. Walk around the impassable on both the left and right edges and
	**    take the shorter of the two paths.
	**
	** 4. Taking the new location as our start location start again with
	**    step #1.
	*/
	while (path.Length < maxlen) {

top_of_list:
		/*
		**	Have we reached the destination already?  If so abort any further
		**	command building.
		*/
		if (startcell == dest) {
			break;
		}

		/*
		**	Find the absolute correct direction to reach the next straight
		** line cell and what cell it is.
		*/
		direction	= CELL_FACING(startcell, dest);
		next			= Adjacent_Cell(startcell, direction);

		/*
		**	If we can move here, then make this our next move.
		*/
		cost = Passable_Cell(next, direction, threat, threshhold);
		if (cost) {
			Register_Cell(&path, next, direction, cost, threshhold);
		} else {
			/*
			**	If the impassable location is actually the destination,
			**	then stop here and consider this "good enough".
			*/
			if (next == dest) break;

			/*
			**	We could not move to the next cell, so follow through the
			**	impassable until we find a passable spot that can be reached.
			** Once we find a passable, figure out the shortest path to it.
			** Since we have variable passable conditions this is not as
			** simple as it used to be.  The limiter loop below allows us to
			** step through ten doughnuts before we give up.
			*/
			for (int limiter = 0; limiter < 5; limiter++) {

				/*
				**	Get the next passable position by zipping through the
				** impassable positions until a passable position is found
				**	or the destination is reached.
				*/
				for (;;) {

					/*
					**	Move one step closer toward destination.
					*/
					newdir	= CELL_FACING(next, dest);
					next		= Adjacent_Cell(next, newdir);

					/*
					** If the cell is passable then we have been completely
					** successful.  If the cell is not passable then continue.
					*/
					if (Passable_Cell(next, FACING_NONE, threat, threshhold)) {
//					if ((Passable_Cell(next, FACING_NONE, threat, threshhold)) || (next == dest)) {
						break;
					}

					/*
					**	If we reached destination while in this loop, we
					**	know that either the destination is impassible (if
					**	we are ignoring) or that we need to up our threat
					** tolerance and try again.
					*/
					if (next == dest) {
						if (threat != -1) {
							switch (threat_stage++) {
								case 0:
									threat = unit_threat >> 1;
									break;

								case 1:
									threat += unit_threat;
									break;

								case 2:
									threat = -1;
									break;
							}
							goto top_of_list;
						}
						goto end_of_list;
					}
				}

				/*
				**	Try to find a path to the passable position by following
				**	the edge of the blocking object in both CLOCKwise and
				**	COUNTERCLOCKwise fashions.
				*/
				int follow_len = maxlen + (maxlen >> 1);

				Mem_Copy(&path, &pleft, sizeof(PathType));
				pleft.Command 	= &moves_left[0];
				pleft.Overlap 	= LeftOverlap;
				Mem_Copy(path.Command, pleft.Command, path.Length);
				Mem_Copy(path.Overlap, pleft.Overlap, sizeof(LeftOverlap));

				// MBL 09.30.2019: We hit a runtime bounds crash where END (-1 / 0xFF) was being poked into +1 just past the end of the moves_right[] array;
				// The FacingType moves_left[] and moves_right[] arrays already have MAX_MLIST_SIZE+2 as their size, which may have been a previous attempted fix;
				// We are now passing MAX_MLIST_SIZE, since the sizeof calculations included the +2 buffering; This code does not exist in Tiberian Dawn.
				#if 0
				left = Follow_Edge(startcell, next, &pleft, COUNTERCLOCK, direction, threat, threat_stage, sizeof(moves_left)/sizeof(moves_left[0]), threshhold);
//				left = Follow_Edge(startcell, next, &pleft, COUNTERCLOCK, direction, threat, threat_stage, follow_len, threshhold);
				#endif
				left = Follow_Edge(startcell, next, &pleft, COUNTERCLOCK, direction, threat, threat_stage, MAX_MLIST_SIZE, threshhold);


				if (left) {
					follow_len = min(maxlen, pleft.Length + (pleft.Length >> 1));
				}

				Mem_Copy(&path, &pright, sizeof(PathType));
				pright.Command = &moves_right[0];
				pright.Overlap = RightOverlap;
				Mem_Copy(path.Command, pright.Command, path.Length);
				Mem_Copy(path.Overlap, pright.Overlap, sizeof(RightOverlap));

				// MBL 09.30.2019: We hit a runtime bounds crash where END (-1 / 0xFF) was being poked into +1 just past the end of the moves_right[] array;
				// The FacingType moves_left[] and moves_right[] arrays already have MAX_MLIST_SIZE+2 as their size, which may have been a previous attempted fix;
				// We are now passing MAX_MLIST_SIZE, since the sizeof calculations included the +2 buffering; This code does not exist in Tiberian Dawn.
				#if 0
				right = Follow_Edge(startcell, next, &pright, CLOCK, direction, threat, threat_stage, sizeof(moves_right)/sizeof(moves_right[0]), threshhold);
//				right = Follow_Edge(startcell, next, &pright, CLOCK, direction, threat, threat_stage, follow_len, threshhold);
				#endif
				right = Follow_Edge(startcell, next, &pright, CLOCK, direction, threat, threat_stage, MAX_MLIST_SIZE, threshhold);

				/*
				**	If we could find a path, break from this loop. Otherwise this
				**	means that we have found a "hole" of passable terrain that
				**	cannot be reached by normal means. Scan forward looking for
				**	the other side of the "doughnut".
				*/
				if (left || right) break;

				/*
				**	If no path can be found to the intermediate cell, then
				**	presume we have found a doughnut of some sort. Scan
				**	forward until the next impassable is found and then
				**	process this loop again.
				*/
				do {

					/*
					**	If we reached destination while in this loop, we
					**	know that either the destination is impassible (if
					**	we are ignoring) or that we need to up our threat
					** tolerance and try again.
					*/
					if (next == dest) {
						if (threat != -1) {
							switch (threat_stage++) {
								case 0:
									threat = unit_threat >> 1;
									break;

								case 1:
									threat += unit_threat;
									break;

								case 2:
									threat = -1;
									break;
							}
							goto top_of_list;
						}
						goto end_of_list;
					}

					newdir	= CELL_FACING(next, dest);
					next		= Adjacent_Cell(next, newdir);
				} while (Passable_Cell(next, newdir, threat, threshhold));
			}

			if (!left && !right) break;

			/*
			**	We found a path around the impassable locations, so figure out
			**	which one was the smallest and copy those moves into the
			**	path.Command array.
			*/
			which = &pleft;
			if (right) {
				which = &pright;
				if (left) {
					if (pleft.Length < pright.Length) {
						which = &pleft;
					} else {
						which = &pright;
					}
				}
			}

			/*
			**	Record as much as possible of the shorter of the two
			**	paths. The trailing EOL command is not copied because
			**	this may not be the end of the find path logic.
			*/
			len = which->Length;
			len = min(len, maxlen);
			if (len > 0) {
				memcpy(&path.Overlap[0], &which->Overlap[0], sizeof(LeftOverlap));
				memcpy(&path.Command[0], &which->Command[0], len * sizeof(FacingType));
				path.Length 		= len;
				path.Cost   		= which->Cost;
				path.LastOverlap 	= -1;
				path.LastFixup	 	= -1;
			} else {
				break;
			}
		}
		startcell = next;
	}

end_of_list:
	/*
	**	Poke in the stop command.
	*/
	if (path.Length < maxlen) {
		path.Command[path.Length++] = END;
	}

	/*
	**	Optimize the move list but only necessary if
	**	diagonal moves are allowed.
	*/
	#ifdef DIAGONAL
		Optimize_Moves(&path, threshhold);
	#endif

	BEnd(BENCH_FINDPATH);

	return(&path);
}


/***********************************************************************************************
 * Follow_Edge -- Follow an edge to get around an impassable spot.                             *
 *                                                                                             *
 * INPUT:   start    -- cell to head from                                                      *
 *                                                                                             *
 *            target   -- Target cell to head to.                                              *
 *                                                                                             *
 *          path     -- Pointer to path list structure.                                        *
 *                                                                                             *
 *          search   -- Direction of search (1=clock, -1=counterclock).                        *
 *                                                                                             *
 *          olddir   -- Facing impassible direction from start.                                *
 *                                                                                             *
 *          callback -- Function pointer for determining if a cell is                          *
 *                      passable or not.                                                       *
 *                                                                                             *
 * OUTPUT:  bool: Could a path be found to the desired cell?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1991  CY : Created.                                                                 *
 *   06/01/1992  JLB : Optimized & commented.                                                  *
 *=============================================================================================*/
bool FootClass::Follow_Edge(CELL start, CELL target, PathType * path, FacingType search, FacingType olddir, int threat, int , int max_cells, MoveType threshhold)
{
	FacingType	newdir;			// Direction of facing before surrounding cell check.
	CELL			oldcell,		// Current cell.
					newcell;		// Tentative new cell.
	int			cost;				// Working cost value.
	int			startx;
	int			starty;
	int			online=true;
	int			targetx;
	int			targety;
	int			oldval = 0;
	int			cellcount=0;
	int			forceout = false;
	FacingType	firstdir = (FacingType)-1;
	CELL			firstcell = -1;
	bool			stepped_off_line = false;
	startx 	= Cell_X(start);
	starty	= Cell_Y(start);
	targetx  = Cell_X(target);
	targety	= Cell_Y(target);

	if (!path) return(false);
	path->LastOverlap = -1;
	path->LastFixup	= -1;

	#ifndef DIAGONAL
		/*
		**	The edge following algorithm doesn't "do" diagonals. Force initial facing
		**	to be an even 90 degree value. Adjust it in the direction it should be
		**	rotating.
		*/
		if (olddir & 0x01) {
			olddir = Next_Direction(olddir, search);
		}
	#endif

	newdir		= Next_Direction(olddir, search);
	oldcell 		= start;
	newcell 		= Adjacent_Cell(oldcell, newdir);

	/*
	**	Continue until we find our target, find our original starting spot,
	**	or run out of moves.
	*/
	while (path->Length < max_cells) {

		/*
		**	Look in all the adjacent cells to determine a passable one that
		**	most closely matches the desired direction (working in the specified
		**	direction).
		*/
		newdir = olddir;
		for (;;) {
			bool	forcefail;		// Is failure forced?

			forcefail = false;

			#ifdef DIAGONAL
				/*
				**	Rotate 45/90 degrees in desired direction.
				*/
				newdir = Next_Direction(newdir, search);

				/*
				**	If facing a diagonal we must check the next 90 degree location
				**	to make sure that we don't walk right by the destination. This
				**	will happen if the destination it is at the corner edge of an
				**	impassable that we are moving around.
				*/
				if (newdir & FACING_NE) {
					CELL	checkcell;		// Non-diagonal check cell.
					//int	x,y;

					checkcell = Adjacent_Cell(oldcell, Next_Direction(newdir, search));

					if (checkcell == target) {

						/*
						**	This only works if in fact, it is possible to move to the
						**	cell from the current location.
						*/
						cost = Passable_Cell(checkcell, Next_Direction(newdir, search), threat, threshhold);
						if (cost) {
							/*
							**	YES! The destination is at the corner of an impassable, so
							**	set the direction to point directly at it and then the
							**	scanning will terminate later.
							*/
							newdir = Next_Direction(newdir, search);
							newcell = Adjacent_Cell(oldcell, newdir);
							break;
						}
					}

					/*
					**	Perform special diagonal check. If the edge follower would cross the
					**	diagonal or fall on the diagonal line from the source, then consider
					**	that cell impassible. Otherwise, the find path algorithm will fail
					**	when there are two impassible locations located on a diagonal
					**	that is lined up between the source and destination location.
					**
					** P.S. It might help if you check the right cell rather than using
					**      the value that just happened to be in checkcell.
					*/

					checkcell = Adjacent_Cell(oldcell, newdir);

					int checkx		= Cell_X(checkcell);
					int checky		= Cell_Y(checkcell);
					int checkval	= Point_Relative_To_Line(checkx, checky, startx, starty, targetx, targety);
					if (checkval && !online) {
						forcefail = ((checkval ^ oldval) < 0);
					} else {
			 			forcefail = false;
					}
					/*
					** The only exception to the above is when we are directly backtracking
					** because we could be trying to escape from a culdesack!
					*/
					if (forcefail && path->Length > 0 && (FacingType)(newdir ^ 4) == path->Command[path->Length - 1]) {
						forcefail = false;
					}
				}

			#else
				newdir = Next_Direction(newdir, search*2);
			#endif

			/*
			**	If we have just checked the same heading we started with,
			**	we are surrounded by impassable characters and we exit.
			*/
			if (newdir == olddir) {
				return(false);
			}

			/*
			**	Get the new cell.
			*/
			newcell = Adjacent_Cell(oldcell, newdir);

			/*
			**	If we found a passable position, this is where we should move.
			*/
			if (!forcefail && ((cost = Passable_Cell(newcell, newdir, threat, threshhold)) != 0)) {
				break;
			} else {
				if (newcell == target) {
					forceout = true;
					break;
				}
			}

		}

		/*
		**	Record the direction.
		*/
		if (!forceout) {
			/*
			** Mark the cell because this is where we need to be.  If register
			** cell fails then the list has been shortened and we need to adjust
			** the new direction.
			*/
			if (!Register_Cell(path, newcell, newdir, cost, threshhold)) {
				/*
				** The only reason we could not register a cell is that we are in
				** a looping situation.  So we need to try and unravel the loop if
				** we can.
				*/
				if (!Unravel_Loop(path, newcell, newdir, startx, starty, targetx, targety, threshhold)) {
					return(false);
				}
				/*
				** Since we need to eliminate a diagonal we must pretend the upon
				** attaining this square, we were moving turned further in the
				** search direction then we really were.
				*/
				newdir = Next_Direction(newdir, (FacingType)(search*2));
			}
			/*
			** Find out which side of the line this cell is on.  If it is on
			** a side, then store off that side.
			*/
			int newx	= Cell_X(newcell);
			int newy	= Cell_Y(newcell);
			int val	= Point_Relative_To_Line(newx, newy, startx, starty, targetx, targety);
			if (val) {
				oldval = val;
				online = false;
			} else {
				online = true;
			}
			cellcount++;
			if (cellcount == MAX_PATH_EDGE_FOLLOW) {
				return(false);
			}
		}

		/*
		**	If we have found the target spot, we are done.
		*/
		if (newcell == target) {
			path->Command[path->Length] = END;
			return(true);
		}

		/*
		**	If we make a full circle back to our original spot, get out.
		*/
		if (newcell == firstcell && newdir == firstdir) {
			return(false);
		}

		if (firstcell == -1) {
			firstcell = newcell;
			firstdir  = newdir;
		}

		/*
		**	Because we moved, our facing is now incorrect. We want to face toward
		**	the impassable edge we are following (well, not actually toward, but
		**	a little past so that we can turn corners). We have to turn 45/90 degrees
		**	more than expected in anticipation of the pending 45/90 degree turn at
		**	the start of this loop.
		*/
		#ifdef DIAGONAL
			olddir = Next_Direction(newdir, (FacingType)(-(int)search*3));
		#else
			olddir = Next_Direction(newdir, (FacingType)(-(int)search*4));
		#endif
		oldcell = newcell;
	}

	/*
	**	The maximum search path is exhausted... abort with a failure.
	*/
	return(false);
}


/***********************************************************************************************
 * Optimize_Moves -- Optimize the move list.                                                   *
 *                                                                                             *
 * INPUT:      char *moves to optimize                                                         *
 *                                                                                             *
 * OUTPUT:     none (list is optimized)                                                        *
 *                                                                                             *
 * WARNINGS:   EMPTY moves are used to hold the place of eliminated                            *
 *             commands. Also, NEVER call this routine with a list that                        *
 *             contains illegal commands. The list MUST be terminated                          *
 *             with a EOL command                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1991  CY : Created.                                                                 *
 *   06/01/1992  JLB : Optimized and commented.                                                *
 *=============================================================================================*/
#define	EMPTY		(FacingType)-2
int FootClass::Optimize_Moves(PathType * path, MoveType threshhold)
//int Optimize_Moves(PathType *path, int (*callback)(CELL, FacingType), int threshold)
{
	/*
	**	Facing command pair adjustment table. Compare the facing difference between
	**	the two commands. 0 means no optimization is possible. 3 means backtracking
	**	so eliminate both commands. Any other value adjusts the first command facing.
	*/
#ifdef DIAGONAL
	static FacingType _trans[FACING_COUNT] = {(FacingType)0, (FacingType)0, (FacingType)1, (FacingType)2, (FacingType)3, (FacingType)-2, (FacingType)-1, (FacingType)0};	// Smoothing.
#else
	static FacingType _trans[FACING_COUNT] = {(FacingType)0, (FacingType)0, (FacingType)0, (FacingType)2, (FacingType)3, (FacingType)-2, (FacingType)0, (FacingType)0};
#endif
	FacingType	* cmd1,		// Floating first command pointer.
					* cmd2,		// Floating second command pointer.
					newcmd;		// Calculated new optimized command.
	FacingType	newdir;		// Tentative new direction for smoothing.
	CELL			cell;			// Working cell (as it moves along path).

	/*
	**	Abort if there is any illegal parameter.
	*/
	if (!path || !path->Command) return(0);

	/*
	**	Optimization loop -- start scanning with the
	**	first pair of commands (if there are at least two
	**	in the command list).
	*/
	path->Command[path->Length] = END;		// Force end of list.

	if (path->Length == 0) return(0);

	cell = path->Start;
	if (path->Length > 1) {
		cmd2 = path->Command + 1;
		while (*cmd2 != END) {

			/*
			**	Set the cmd1 pointer to point to the valid command closest, but
			**	previous to cmd2. Be sure not to go previous to the head of the
			**	command list.
			*/
			cmd1 = cmd2-1;
			while (*cmd1 == EMPTY && cmd1 != path->Command) {
				cmd1--;
			}

			/*
			**	If there isn't any valid previous command, then bump the
			**	cmd pointers to the next command pair and continue...
			*/
			if (*cmd1 == EMPTY) {
				cmd2++;
				continue;
			}

			/*
			**	Fetch precalculated command change value. 0 means leave
			**	command set alone, 3 means backtrack and eliminate two
			**	commands. Any other value is new direction and eliminate
			**	one command.
			*/
			newcmd = (FacingType)(*cmd2 - *cmd1);
			if (newcmd < FACING_N) newcmd = (FacingType)(newcmd + FACING_COUNT);
			newcmd = _trans[newcmd];

			/*
			**	Check for backtracking. If this occurs, then eliminate the
			**	two commands. This is the easiest optimization.
			*/
			if (newcmd == FACING_SE) {
				*cmd1 = EMPTY;
				*cmd2++ = EMPTY;
				continue;
			}

			/*
			**	If an optimization code was found the process it. The command is a facing
			**	offset to more directly travel toward the immediate destination cell.
			*/
			if (newcmd) {

				/*
				**	Optimizations differ when dealing with diagonals. Especially when dealing
				**	with diagonals of 90 degrees. In such a case, 90 degree optimizations can
				**	only be optimized if the intervening cell is passable. The distance travelled
				**	is the same, but the path is less circuitous.
				*/
				if (*cmd1 & FACING_NE) {

					/*
					**	Diagonal optimizations are always only 45
					**	degree adjustments.
					*/
					newdir = Next_Direction(*cmd1, (newcmd < FACING_N) ? (FacingType)-1 : (FacingType)1);

					/*
					**	Diagonal 90 degree changes can be smoothed, although
					**	the path isn't any shorter.
					*/
					if (ABS((int)newcmd) == 1) {
						if (Passable_Cell(Adjacent_Cell(cell, newdir), newdir, -1, threshhold)) {
							*cmd2 = newdir;
							*cmd1 = newdir;
						}
						// BOB 16.12.92
						cell = Adjacent_Cell(cell, *cmd1);
						cmd2++;
						continue;
					}
				} else {
					newdir = Next_Direction(*cmd1, newcmd);
				}

				/*
				**	Allow shortening turn only on right angle moves that are based on
				**	90 degrees. Always allow 135 degree optimizations.
				*/
				*cmd2 = newdir;
				*cmd1 = EMPTY;

				/*
				**	Backup what it thinks is the current cell.
				*/
				while (*cmd1 == EMPTY && cmd1 != path->Command) {
					cmd1--;
				}
				if (*cmd1 != EMPTY) {
					cell = Adjacent_Cell(cell, Next_Direction(*cmd1, FACING_S));
				} else {
					cell = path->Start;
				}
				continue;
			}

			/*
			**	Since we could not make an optimization, we move our
			**	head pointer forward.
			*/
			cell = Adjacent_Cell(cell, *cmd1);
			cmd2++;
		}
	}

	/*
	**	Pack the command list to remove any EMPTY command entries.
	*/
	cmd1 = path->Command;
	cmd2 = path->Command;
	cell = path->Start;
	path->Cost = 0;
	path->Length = 0;
	while (*cmd2 != END) {
		if (*cmd2 != EMPTY) {
			cell = Adjacent_Cell(cell, *cmd2);
			path->Cost+= Passable_Cell(cell, *cmd2, -1, threshhold);
			path->Length++;
			*cmd1++ = *cmd2;
		}
		cmd2++;
	}
	path->Length++;
	*cmd1 = END;
	return(path->Length);
}


CELL FootClass::Safety_Point(CELL src, CELL dst, int start, int max)
{
	FacingType dir;
	CELL		  next;
	int 		  lp;

	dir = (FacingType)(CELL_FACING(src, dst) ^ 4) - 1;

	/*
	** Loop through the different acceptable distances.
	*/
	for (int dist = start; dist < max; dist ++) {

		/*
		** Move to the starting location.
		*/
		next = dst;

		for (lp = 0; lp < dist; lp ++) {
			next = Adjacent_Cell(next, dir);
		}

		if (dir & 1) {
			/*
			** If our direction is diagonal than we need to check
			** only one side which is as long as both of the old sides
			** together.
			*/
			for (lp = 0; lp < dist << 1; lp ++) {
				next = Adjacent_Cell(next, dir + 3);
				if (!Can_Enter_Cell(next)) {
					return(next);
				}
			}
		} else {
			/*
			** If our direction is not diagonal than we need to check two
			** sides so that we are checking a corner like location.
			*/
			for (lp = 0; lp < dist; lp ++) {
				next = Adjacent_Cell(next, dir + 2);
				if (!Can_Enter_Cell(next)) {
					return(next);
				}
			}

			for (lp = 0; lp < dist; lp ++) {
				next = Adjacent_Cell(next, dir + 4);
				if (!Can_Enter_Cell(next)) {
					return(next);
				}
			}
		}
	}
	return(-1);
}




int FootClass::Passable_Cell(CELL cell, FacingType face, int threat, MoveType threshhold)
{
	MoveType move = Can_Enter_Cell(cell, face);

	if (move < MOVE_MOVING_BLOCK && Distance(Cell_Coord(cell)) > 0x0100) threshhold = MOVE_MOVING_BLOCK;

	if (move > threshhold) return(0);

	if (Session.Type == GAME_NORMAL) {
		if (threat != -1) {
			if (::Distance(Cell_Coord(cell), Cell_Coord(DestLocation)) > (THREAT_THRESHOLD * CELL_LEPTON_W)) {
//			if (Map.Cell_Distance(cell, DestLocation) > THREAT_THRESHOLD) {
				if (Map.Cell_Threat(cell, Owner()) > threat)
					return(0);
			}
		}
	}

	static int _value[MOVE_COUNT] = {
		1,			//	MOVE_OK
		1,			//	MOVE_CLOAK
		3,			//	MOVE_MOVING_BLOCK
		8,			//	MOVE_DESTROYABLE
		10,		//	MOVE_TEMP
		0			//	MOVE_NO
	};
	return(_value[move]);
}

