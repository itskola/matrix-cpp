# matrix-cpp

<h3>Small implementation of Matrices in C++ to simplify process of working with them.</h3>
<div>
  To use this class, add it directly into your project (or create library).
  <br/>
  Tested on Windows 10.
</div>

### Example
```
Matrix A = Matrix("matrixA.txt");
A.print();

Matrix B = Matrix("matrixB.txt");
B.print();

std::cout << "==============" << std::endl;

Matrix C = A + B; // A * B ...
C.print();
```

### Supports:
<table>
  <tr>
    <td>addition</td>
    <td><b>A + B</b></td>
  </tr>
  <tr>
    <td>subtraction</td>
    <td><b>A - B</b></td>
  </tr>
  <tr>
    <td>multiplication</td>
    <td><b>A * B, scalar * A</b></td>
  </tr>
  <tr>
    <td>transpose</td>
    <td><b>~A</b></td>
  </tr>
  <tr>
    <td>inverse (if there is one)</td>
    <td><b>A.inverse()</b></td>
  </tr>
</table>
