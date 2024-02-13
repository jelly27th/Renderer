#ifndef _MACRO_H
#define _MACRO_H

#define Negative_Infinity (-9999.0)

/*
 * Mapping one interval to another interval, for more information see
 * https://blog.csdn.net/Touch_Dream/article/details/62076236
 */
#define mapping_interval(Omin, Omax, Nmin, Nmax, a) \
    ((float)Nmin + ((Nmax - Nmin) / (float)(Omax - Omin)) * (a - Omin))

#endif