# Projeto final das disciplinas Introdução as Técnicas de Programação(ITP) e Práticas e Técnicas de Programação(PTP)

<h2>Autores</h2
<ul>
  <li><a href="https://github.com/cleiton-lima">Antonio Cleiton Ribeiro de Lima</a></li>
  <li><a href="https://github.com/tiagojordao">Tiago Jordão Pereira Cabral de Morais</a></li> 
</ul>

### Como compilar o projeto
```sh
$  gcc catarata.c -o catarata -lm
```

```sh
$  ./catarata -i Catarata.ppm -f ppm -o diagnostico.txt
```
<i>É importante ressaltar que pode ser necessário alterar o nome da imagem, podendo ser "Catarata2.ppm", por exemplo.</i>

<h3>Diagnóstico de Catarata</h3>

### O que foi feito:
<ul>
  <li>Leitura das imagens de acordo com os tipos de dados suportados em C</li>
  <li>Modularização do programa</li>
  <li>Leitura de imagens PPM</li>
  <li>Transformação das imagens em tons de cinza</li>
  <li>Filtro Gaussiano</li>
  <li>Binarização</li>
  <li>Transformada de Hough</li>
  <li>Filtro de Sobel</li>
  <li>Segmentação da pupila</li>
  <li>Diagnóstico da catarata</li>
  <li>Comentários no código</li>
</ul>

### O que não foi feito:
<ul>
  <li>A imagem Normal2 não está sendo reconhecida como deveria</li>
</ul>

### O que seria feito diferentemente:
<ul>
  <li>Uma estruturação mais bem elaborada da modularização</li>
  <li>Fazer o projeto de acordo com as boas práticas de programação, fazendo o uso do inglês tanto no código quanto nos comentários</li>
  <li>Refatorar algumas funções para que o tempo de execução do código fosse menor como por exemplo, na transformada de Hough</li>
</ul>

### Funcionalidade Extra
<ul>
  <li>Isolar a parte do <strong>flash</strong> das imagens</li>
</ul>
