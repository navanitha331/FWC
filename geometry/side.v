module Matsub (
  input signed [7:0] A[1:0][0:0],
  input signed [7:0] B[1:0][0:0],
  input signed [7:0] C[1:0][0:0],
  output reg signed [7:0] sub[1:0][2:0]
  );

  integer i,j;
  
  always @* begin
    for (i = 0; i < 2; i = i+1) begin
        sub[i][0] = A[i][0] - B[i][0];
        sub[i][1] = B[i][0] - C[i][0];
        sub[i][2] = C[i][0] - A[i][0];
         end
  end
endmodule

module trans1(
       input signed [7:0] A[1:0][0:0],
       input signed [7:0] B[1:0][0:0],
       input signed [7:0] C[1:0][0:0],
       input signed [7:0] sub[1:0][2:0], // Input sub array from Matsub
       output reg signed [7:0] Q[2:0][1:0] 
 );
 integer i, j;
always @* begin
 for(i=0;i<3;i=i+1) begin
  for(j=0;j<2;j=j+1) begin
    Q[0][j] = sub[j][0];
    Q[1][j] = sub[j][1];
    Q[2][j] = sub[j][2];
    end
 end
end
endmodule
 
module matdot(
       input signed [7:0] A[1:0][0:0],
       input signed [7:0] B[1:0][0:0],
       input signed [7:0] C[1:0][0:0],
       input signed [7:0] sub[1:0][2:0], // Input sub array from Matsub
       input signed [7:0] Q[2:0][1:0], 
       output reg signed [7:0] temp1,
       output reg signed [7:0] temp2,
       output reg signed [7:0] temp3
 );


integer i;
  always @* begin
  temp1=0;
  temp2=0;
  temp3=0;
     for(i=0;i<2;i=i+1) begin
         temp1 =temp1 + (sub[i][0] * Q[0][i]);
         temp2 =temp2 + (sub[i][1] * Q[1][i]);
         temp3 =temp3 + (sub[i][2] * Q[2][i]);
         
           end
        end
       
endmodule

module dis(
       input signed [7:0] A[1:0][0:0],
       input signed [7:0] B[1:0][0:0],
       input signed [7:0] C[1:0][0:0],
       input signed [7:0] sub[1:0][2:0], // Input sub array from Matsub
       input signed [7:0] Q[2:0][1:0], 
       input signed [7:0] temp1,
       input signed [7:0] temp2,
       input signed [7:0] temp3,
       output reg signed [7:0] side_AB,
       output reg signed [7:0] side_BC,
       output reg signed [7:0] side_CA   
 );
  always @* begin
  side_AB = $sqrt(temp1);
  side_BC = $sqrt(temp2);
  side_CA = $sqrt(temp3);
  end    
endmodule

       
 
