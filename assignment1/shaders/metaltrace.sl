// modified from example in https://renderman.pixar.com/view/ray-traced-shading
// code also taken from metal.sl and shinymetal.sl
surface metaltrace (float Ka=1, Ks=1, Kr = 1, Kd = .5, roughness=.1;
            string texturename = "";)
{
        normal Nn = normalize(N);
        vector In = normalize(I);
            color  Cr;

        color spec = Cs;


        if (Nn.In < 0) {
                vector r = reflect(In,Nn);
                spec += trace(P, r);
        }
        
        vector norm = faceforward(Nn,In);


        if (texturename != "") {
                vector D = reflect(I, norm) ;
                D = vtransform ("world", D);
                Cr = Kr * color environment(texturename, D);
        } else {
                Cr = 0.;
        }
        Ci += 0.2 * specular(norm, -In, roughness);  
        
        Ci = Os * ( Cr*(Ka*ambient() + Kd*diffuse(norm) + Ks*spec ));
        Oi = Os;
}