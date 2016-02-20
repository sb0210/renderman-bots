// modified fromhttps://renderman.pixar.com/view/global-illumination-and-all-that#Single_Bounce_Global_Illuminatio
surface bleedwall(float Ka=1, Kd=1, samples = 25;)
{
        vector Nf = faceforward(normalize(N),I);


        // Compute direct illumination
        color direct = diffuse(Nf) ;
        
        // Compute soft indirect illumination (if diff. depth < maxdiffusedepth)
        color indirect = indirectdiffuse(P, Nf, samples);
        
        // Set Ci and Oi
        Ci = (Kd * (direct + indirect) + Ka*ambient()) * Cs * Os;
        Oi = Os;
}