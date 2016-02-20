					surface simplemirror ( )
{
        normal Nn = normalize(N);
        vector In = normalize(I);
        color reflection = Cs;
        
        if (Nn.In < 0) {
                vector reflDir = reflect(In,Nn);
                reflection = trace(P, reflDir);
        }
        
        Ci = Os * reflection;
        Oi = Os;
}