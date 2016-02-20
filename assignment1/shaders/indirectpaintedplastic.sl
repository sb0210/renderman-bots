// modified from renderman's `paintedplastic` shader to accept indirect diffuse illumination

/* $Id: //depot/branches/rmanprod/rman-20.0/shaders/paintedplastic.sl#1 $  (Pixar - RenderMan Division)  $Date: 2015/04/23 $ */
/*
** Copyright (c) 1999 PIXAR.  All rights reserved.  This program or
** documentation contains proprietary confidential information and trade
** secrets of PIXAR.  Reverse engineering of object code is prohibited.
** Use of copyright notice is precautionary and does not imply
** publication.
**
**                      RESTRICTED RIGHTS NOTICE
**
** Use, duplication, or disclosure by the Government is subject to the
** following restrictions:  For civilian agencies, subparagraphs (a) through
** (d) of the Commercial Computer Software--Restricted Rights clause at
** 52.227-19 of the FAR; and, for units of the Department of Defense, DoD
** Supplement to the FAR, clause 52.227-7013 (c)(1)(ii), Rights in
** Technical Data and Computer Software.
**
** Pixar
** 1001 West Cutting Blvd.
** Richmond, CA  94804
*/
surface
indirectpaintedplastic( float Ks=.7, Kd=.5, Ka=1, roughness=.5; color specularcolor=1;
     string texturename = ""; float samples=25;)
{
    normal Nf;
    vector V;
    color Ct;

    Nf = faceforward( normalize(N), I );
    V = -normalize(I);

    if (texturename != "") {
    Ct = color texture(texturename);
    } else {
    Ct = 1.0;
    }

    Oi = Os;
    Ci = Os * ( Cs * Ct * (Ka*ambient() + Kd*(diffuse(Nf) + indirectdiffuse(P, Nf, samples) ) ) +
        specularcolor * Ks * specular(Nf,V,roughness) );
}
