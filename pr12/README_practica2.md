# Práctica 2: flor mecánica

La aplicación reúne los dos modelos requeridos. A la izquierda se dibuja una hoja paramétrica indexada en `igvMallaTriangulos`, con arrays de vértices, índices y normales calculadas promediando las caras vecinas. A la derecha está la flor articulada, hecha con esferas GLUT y cilindros/esferas GLU.

## Grafo de escena

```text
Escena
├── Hoja paramétrica (transformación completa: T-R-S)
└── Flor (transformación completa: T-R-S)
    ├── Maceta
    │   ├── Cuerpo (cilindro troncocónico GLU)
    │   └── Tierra (esfera GLUT escalada)
    └── Tallo articulado (inclinación X/Z y elevación)
        ├── Cilindro del tallo
        ├── Rama/hoja izquierda (rotación independiente)
        ├── Rama/hoja derecha (rotación independiente)
        └── Cabeza giratoria
            ├── Centro (esfera)
            ├── 8 pétalos hermanos (esferas GLU escaladas, apertura común)
            └── 6 puntos decorativos (esferas)
```

La jerarquía tiene más de cuatro niveles y ramas hermanas, por lo que no es lineal. Hay movimientos independientes para la inclinación del tallo, giro de cada rama, giro de la cabeza y apertura/cierre de los pétalos; la elevación de la cabeza añade traslación. Los ángulos se limitan al arrastrar las ramas y pétalos.

## Controles

| Tecla / acción | Función |
| --- | --- |
| `1` | Seleccionar el tallo |
| `2`, `3` | Seleccionar rama/hoja izquierda o derecha |
| `4` | Seleccionar cabeza giratoria |
| `5`–`8` | Seleccionar pétalos para mover su apertura |
| Flechas | Mover la parte seleccionada; si no hay parte seleccionada, trasladar el modelo completo |
| `0`, `9` | Seleccionar la hoja paramétrica o la flor para las transformaciones completas |
| `x/X`, `y/Y`, `z/Z` | Rotar el modelo completo seleccionado |
| `u/U`, `s/S` | Trasladar verticalmente / escalar el modelo completo |
| `c` | Alternar entre control de cámara y control de objetos para las flechas |
| `+`, `-`, `f/F`, `b/B` | Zoom y ajuste de planos de recorte |
| `g` | Alternar sombreado Gouraud y plano |
| `e` | Mostrar u ocultar los ejes |
| `m` | Alternar interacción con ratón / teclado |
| Clic y arrastre izquierdo | Seleccionar por buffer de color y mover la parte de la flor |
| `a` | Activar o pausar la animación automática de la flor y la hoja |
| `o` | Activar o pausar la órbita automática de cámara |
| `v` | Alternar vista normal con inset de planta y cuatro viewports |
| Botón derecho | Menú de modo de entrada, animación, órbita, sombreado y vistas |
| `Esc` / `q` | Salir |

En la vista inicial la cámara panorámica ocupa la ventana y la vista de planta aparece arriba a la derecha. Con `v`, la ventana se divide en alzado (arriba izquierda), planta (arriba derecha), perfil (abajo izquierda) y panorámica (abajo derecha). El buffer de selección dibuja colores RGB sin iluminación ni dithering y lee el píxel bajo el cursor, invirtiendo la coordenada vertical de GLUT.

## Archivos añadidos

- `igvMallaTriangulos.h/.cpp`: malla indexada y construcción de una hoja paramétrica.
- `igvEscena3D.h/.cpp`: modelos, jerarquía articulada, transformaciones, sombreado y animación.
- `igvInterfaz.h/.cpp`: cámaras, viewports, teclado, menú GLUT y selección/arrastre por color.
