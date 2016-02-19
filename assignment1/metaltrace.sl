// modified from example in https://renderman.pixar.com/view/ray-traced-shading
surface metaltrace ( )
{
        normal Nn = normalize(N);
        vector In = normalize(I);
        Ci = Cs;
        
        if (Nn.In < 0) {
                vector r = reflect(In,Nn);
                Ci += trace(P, r);
        }
        
        vector v = faceforward(Nn,In);
        Ci += 0.2 * specular(v, -In, 0.05);  
        
        Ci *= Os;
        Oi = Os;
}