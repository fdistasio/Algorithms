function [x_new] = my_jacobi(A, b, x_old)

    n = length(b);
    x_new = zeros(n, 1);
    
    for i=1:n
        s = 0;
        for j=1:i-1
            s = s + (A(i, j) * x_old(j));
        end
    
        for j=i+1:n
            s = s + (A(i, j) * x_old(j));
        end
        x_new(i) = (b(i) - s) / A(i, i);
    end

end