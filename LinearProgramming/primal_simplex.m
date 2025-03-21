function [B, x, y, status] = primal_simplex(A, b, c, B)
    
    % Assume that B is primal feasible
    n = length(b);
    m = length(c);

    while(true)
        
        lambda = +inf;
        y = zeros(1, n);
        u = zeros(m, 1);
        
        A_B = A(B, :);
        b_B = b(B, :);
        A_N = A;
        A_N(B, :) = [];

        B = sort(B);
        
        % Primal solution
        A_B_inv = inv(A_B);
        x = A_B_inv * b_B;

        % Dual solution
        for i=1:n
            for j=1:m 
                if(i == B(j)) 
                    y(i) = c * A_B_inv(:, j);
                end
            end
        end
       
        % End if y is dual feasible
        if(all(y >= 0))
            status = "optimal";
            break;
        end
       
        % Outgoing index h
        for i=1:n
            if(y(i) < 0)
                h = i;
                break;
            end
        end
        
        % Growth direction xi
        for i=1:m 
            if (h == B(i))
                u(i) = 1;
            end
        end
        xi = -A_B_inv * u;
        
        % End if A_N*xi is an unbounded growth direction
        if(all(A_N*xi <= 0))
            status = "Primal unbounded";
            break;
        end

        % Incoming index k
        for i=1:n-m 
            cur = A_N(i, :) * xi;
            if (all(cur > 0)) 
                lambda_i = (b(i) - A_N(i, :) * x) / cur;

                if(lambda_i < lambda)
                    lambda = lambda_i;
                    k = i;
                end
            end 
        end
        
        % Change Basis
        B(find(B == h)) = k;

    end

end