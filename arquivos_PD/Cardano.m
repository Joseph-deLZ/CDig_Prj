a = e*f;
b = (K*b_prime*c_prime + e + f);
c = (K*b_prime*A + c_prime + K*b_prime + 1);
d = K*b_prime*A;

p = (3*a*c - b^2) / (3*a^2);
q = (2*b^3 - 9*a*b*c + 27*a^2*d) / (27*a^3);

% Para ver a expressão simplificada:
simplify(p)
simplify(q)