function [x] = gauss_lu(A, b)
    
    n = length(b);
    m = zeros(n, 1);

    for i=1:n-1
        
        % Multipliers calculation
        for j=i+1:n
            m(j) = A(j, i) / A(i, i);
            A(j, i) = m(j);
        end
        
        % Gaussian elimination
        for j=i+1:n
            for k=i+1:n
                A(j, k) = A(j, k) - m(j) * A(i, k);
            end
            b(j) = b(j) - m(j) * b(i);
        end

    end
    
    U = triu(A);
    L = tril(A);

    for i=1:n
        L(i, i) = 1;
    end
    
    % Solve Ax = b <=> LUx = b <=> {Ly = b ; Ux = y}
    y = tri_low_solver(L, b);
    x = tri_up_solver(U, y);

end
