# CG-UFABC-FPS3D

Treine sua mira com o Aim Trainer 3D!

Baseado no [Aim Lab](https://aimlab.gg/), o Aim Trainer 3D é uma aplicação desenvolvida com C++.

**[Clique aqui para jogar!](https://fernandokenjii.github.io/aim_trainer_3D/)**

### Dependência

Foi utilizada a biblioteca [ABCg](https://github.com/hbatagelo/abcg)

Desenvolvida por Harlen Batagelo


### Autores: 
  Fernando Kenji - RA: 11201722146
  
  Lucas Yudi - RA: 11201812210


### Objetivo
Acerte a maior quantidade de alvos em 30 segundos.


### Controles
- Mirar: Mover o mouse;
- Atirar: Qualquer botão do mouse;

Além disso, é possível alterar as seguintes propriedades do jogo:
- Exibir/Esconder arma: Seta para esquerda ```←``` a qualquer momento do jogo.
- Habilitar/Desabilitar som: Seta para direita ```→``` a qualquer momento do jogo.

No menu inicial ou no fim de jogo, pressione ```espaço``` para iniciar uma nova partida.

No fim de jogo, pressione a tecla ```ESC``` para exibir o cursor e a seta para cima ```↑``` para trocar o som da arma.


### Tela
A primeira tela é o menu inicial, onde são exibidas instruções do jogo.

Durante o jogo, o cursor é desabilitado e é exibido o tempo restante.

No fim de jogo, são exibidas as seguintes estatísticas:
- Disparos realizados;
- Alvos acertados;
- Precisão;
- Alvos acertados por segundo.

Além de ser possível alterar algumas configurações, como citado em [Controles](#controles)

## Projeto
O projeto está dividido em dependências (```assets```) e código.

### Dependências
- [Texturas](assets/maps): responsáveis por definir cores e padrões dos ```Modelos```. São imagens mapeando texturas e normais.
- [Shaders](assets/shaders): programas responsáveis por tratar sombreamento de objetos. Foi utilizado o modelo de reflexão de [Blinn Phong](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model).
- [Modelos](assets/models): define os vértices dos objetos, assim como propriedade dos materiais destes, como cores, reflexão, iluminação e texturas utilizadas.
- [Sons](assets/sounds): sons do jogo, em específico, dos disparos.

### Código
Está dividido entre as classes:
- ```Camera```: responsável pela projeção de um visualizador na aplicação. É possível realizar movimentos ```pan```(alteração do ângulo paralelo ao plano) e ```tilt``` (alteração do ângulo ortogonal ao plano)
- ```Gamedata```: define o estado e informações do jogo.
- ```Model```: responsável por definir a renderização do modelo 3D, ou seja, calcula normais e tangentes, carrega e configura vértices e os renderiza.
- ```Openglwindow```: responsável por desenhar em tela a aplicação e receber eventos para interação com a aplicação. Reúne as demais classes para formação do jogo.
- ```Targets```: modelo referente aos alvos. Contém informações de quais possíveis posições do alvo, lógica de adição/remoção de alvos e quantidade simultânea de alvos.

### Técnicas 

Além do descrito acima, algumas técnicas valem mencionar:
- Definimos posições fixas para os targets, dese modo não é preciso fazer cálculo de posição dos targets para desenhar um novo, apenas verificar qual posição não foi ocupada.
- Para saber se um target foi acertado, foi calculada a equação da reta no espaço tridimensional entre a posição da câmera e para onde ela esta "olhando". Depois verificou-se se a distância entre as posições dos targets e a reta é menor que seu raio.
- Foi utilizado, de forma adicional, funções do ImGui para lidar com eventos de input. Quando uma tela era desenhada pelo ImGui, as funções de evento utilizadas do SDL não funcionavam corretamente

### Auxílio
- Arquivo do objeto 3d, .mtl, textura e normais da arma foram baixados do [Open 3d model](https://open3dmodel.com/pt/3d-models/scifi-handgun_39931.html)
- Objeto 3d e .mtl do quarto e dos targets foram criados no [Blender](https://www.blender.org/)
- Texturas e normais do quarto e targets foram baixados do [Poliigon](https://www.poliigon.com/textures/free)
- Arquivos de som baixados do [Mixkit](https://mixkit.co/free-sound-effects/gun/) e [Videvo](https://www.videvo.net/sound-effect/gun-shot-single-shot-in-pe1097906/246309/)
