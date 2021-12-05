# CG-UFABC-FPS3D

Treine sua mira com o Aim Trainer 3D!

Baseado no [Aim Lab](https://aimlab.gg/), o Aim Trainer 3D é uma aplicação desenvolvida com C++, a partir da biblioteca [ABCg](https://github.com/hbatagelo/abcg).

### Autores: 
  Fernando Kenji - RA: 11201722146
  Lucas Yudi - RA: 11201812210

### Objetivo
Acerte a maior quantidade de alvos em 30 segundos.


### Controles
- Mirar: Mover o mouse;
- Atirar: Qualquer botão do mouse;

Além disso, é possível alterar as seguintes propriedades do jogo:
- Exibir/Esconder arma: Seta para esquerda ```←```
- Habilitar/Desabilitar som: Seta para esquerda ```→```

No menu/fim de jogo, pressione ```espaço``` para iniciar uma nova partida.

No fim de jogo, pressione a tecla ```ESC``` para exibir o cursor e a seta para cima ```↑``` para trocar o som da arma.


### Tela
A primeira tela é o menu, onde são exibidas instruções do jogo.

Durante o jogo, o cursor é desabilitado e é exibido o tempo restante.

No fim de jogo, são exibidas as seguintes estatísticas:
- Disparos realizados;
- Alvos acertados;
- Precisão;
- Alvos acertados por segundo.

Além de ser possível alterar algumas configurações, como citado em [Controles](#controles)

## Projeto
O projeto está dividido em dependências (```assets```) e código.

Nas dependências, temos:
- [Texturas](assets/maps) responsáveis por definir cores e padrões dos ```Modelos```. São imagens mapeando texturas e normais.
- [Shaders](assets/shaders) programas responsáveis por tratar sombreamento de objetos. Foi utilizado o modelo de reflexão de [Blinn Phong](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model).
- [Modelos](assets/models) define os vértices dos objetos, assim como propriedade dos materiais destes, como cores, reflexão, iluminação e texturas utilizadas.
- [Sons](assets/sounds) Sons do jogo, em específico, dos disparos.

### Código
Está dividido entre as classes:
- ```camera```: Responsável pela projeção de um visualizador na aplicação. É possível realizar movimentos ```pan```(alteração do ângulo paralelo ao plano) e ```tilt``` (alteração do ângulo ortogonal ao plano)
- ```gamedata```: Define o estado e informações do jogo.
- ```model```: Responsável por definir a renderização do modelo 3D, ou seja, calcula normais e tangentes, carrega e configura vértices e os renderiza.
- ```openglwindow```: Responsável por desenhar em tela a aplicação e receber eventos para interação com a aplicação. Reúne as demais classes para formação do jogo.
- ```targets```: Modelo referente aos alvos. Contém informações de quais possíveis posições do alvo, lógica de adição/remoção de alvos e quantidade simultânea de alvos.

