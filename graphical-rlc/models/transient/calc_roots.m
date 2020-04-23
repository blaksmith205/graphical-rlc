function roots = calc_roots(R, L, C, isSeries)
if isSeries == true
    a = R/(2*L);
else
    a = 1/(2*R*C);
end
w0 = 1/sqrt(L*C);
s1 = -a + sqrt(a^2-w0^2);
s2 = -a - sqrt(a^2-w0^2);
wd = 0;
if ~isreal(s1) || ~isreal(s2)
    wd = sqrt(w0^2 - a^2);
end
roots = [s1 s2 wd a];
end
    