// modified from example in https://renderman.pixar.com/view/ray-traced-shading
// code also taken from metal.sl and shinymetal.sl
surface metaltrace (float Ka=1, Ks=.2, Kr = 1, Kd = .2, roughness=.1;
            string texturename = ""; float mirror = 0; color nontexturecolor = 1)
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

        spec += 1 * specular(norm, -In, roughness);  

        if (texturename != "") {
                vector D = reflect(I, norm) ;
                D = vtransform ("world", D);
                Cr = Kr * color environment(texturename, D);
                Ci = Os * ( Cr*(Ka*ambient() + Kd*diffuse(norm) + Ks*spec )  );
        } else {
            Cr = nontexturecolor;
            Ci = Os * ( Cr*(Ka*ambient() + Kd*diffuse(norm) ) + Ks*spec );
        }
        
        
        Oi = Os;
}