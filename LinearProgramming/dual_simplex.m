function [B, x, y, status] = dual_simplex(A, b, c, B)
    
    % Assume that B is dual feasible
    n = length(b);
    m = length(c);

    while(true)
        
        teta = +inf;
        y = zeros(1, n);
        
        A_B = A(B, :);
        b_B = b(B, :);
        A_N = A;
        A_N(B, :) = [];
        b_N = b;
        b_N(B, :) = [];

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
       
        % End if x is primal feasible
        A_N_x = A_N*x;
        if(all(A_N_x <= b_N))
            status = "optimal";
            break;
        end
       
        % Incoming index k
        for i=1:n-m
            if(A_N_x(i) > b_N(i))
                k = i;
                break;
            end
        end
        
        % eta_B
        eta_B = A(k, :) * A_B_inv;
        
        % End if Primal is empty
        if(all(eta_B <= 0))
            status = "Primal empty";
            break;
        end

        % Outgoing index h
        for i=1:n
            if(y(i) == 0) 
                continue;
            end
            
            if (eta_B(i) > 0) 
                teta_i = y(i)/eta_B(i);

                if(teta_i < teta)
                    teta = teta_i;
                    h = i;
                end
            end 
        end
        
        % Change Basis
        B(find(B == h)) = k;

    end

end