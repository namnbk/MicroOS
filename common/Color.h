#ifndef COLOR_H
#define COLOR_H

/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/*          ___  ___                                                    */
/*    _  _ / _ \/ __|           µ (Micro) Operating System              */
/*   | || | (_) \__ \           Copyright (C) 2007 raodm@muohio.edu     */
/*   | .__|\___/|___/           Miami University                        */
/*   |_|                        Oxford, OH                              */
/*                                                                      */
/*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

/** \def MAKE_COLOR

    \brief Macro to combine a foreground (4-bits) and background
    (4-bits) to generate a single color.

    This macro essentially shifts the background to the left by 4 bits
    and adds the forground value to it.  The color codes for
    foreground and background colors are shown below:

    <TABLE>
      <TR><TD>Color</TD><TD>Code</TD> <TD>Color</TD><TD>Code</TD></TR>

      <TR><TD>Black</TD><TD>0</TD> <TD>dark gray</TD><TD>8</TD></TR>

      <TR><TD>Blue</TD><TD>1</TD> <TD>Bright Blue</TD><TD>9</TD></TR>

      <TR><TD>Green</TD><TD>2</TD> <TD>Bright Green</TD><TD>10</TD></TR>

      <TR><TD>Cyan</TD><TD>3</TD> <TD>Bright Cyan</TD><TD>11</TD></TR>

      <TR><TD>Red</TD><TD>4</TD> <TD>Bright Red</TD><TD>12</TD></TR>      

      <TR><TD>Purple</TD><TD>5</TD> <TD>Pink</TD><TD>13</TD></TR>

      <TR><TD>Brown</TD><TD>6</TD> <TD>Yellow</TD><TD>14</TD></TR>

      <TR><TD>gray</TD><TD>7</TD> <TD>white</TD><TD>15</TD></TR>
      
    </TABLE>
 */
#define MAKE_COLOR(foreground, background) ((background << 4) + foreground)

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define PURPLE          5
#define BROWN           6
#define GRAY            7

#define DARK_BLACK      8
#define BRIGHT_BLUE     9
#define BRIGHT_GREEN   10
#define BRIGHT_CYAN    11
#define BRIGHT_RED     12
#define BRIGHT_PURPLE  13
#define YELLOW         14
#define WHITE          15

#endif
