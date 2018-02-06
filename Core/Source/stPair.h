// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_PAIR_H
#define ST_PAIR_H

#define ST_PAIR_T(T1, T2)                         struct { T1 first; T2 second; }
#define ST_NAMED_PAIR_T(T1, T2, A1, A2)           struct { T1 A1; T2 A2; }
#define stMakePair(X, Y)                          {(X), (Y)}

#define ST_TRIPLE_T(T1, T2, T3)                   struct { T1 first; T2 second; T3 third; }
#define ST_NAMED_TRIPLE_T(T1, T2, T3, A1, A2, A3) struct { T1 A1; T2 A2; T3 A3; }
#define stMakeTriple(X, Y)                        { (X), (Y), (Z) }

#endif
