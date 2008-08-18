package Formularios;

import analizador.Canal;
import analizador.ControlCanal;
import analizador.ControlMonitor;
import analizador.Dibujo;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

public class VentanaPrincipal extends javax.swing.JFrame {
    private Canal canal[];
    private Dibujo dibujo[];
    private ControlCanal controlCanal[];
    private ControlMonitor controlMonitor;
    
    public VentanaPrincipal() {
        initComponents();
        initComponentsPropio();
    }
    
    private void initComponentsPropio() {
        
        GridBagLayout gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();
        panelDibujos.setLayout(gbl);
        controlMonitor = new ControlMonitor();
        dibujo = new Dibujo[8];
        controlCanal = new ControlCanal[8];
        canal = new Canal[8];
        
        for (int i=0;i<8;i++){
            canal[i] = new Canal();
            dibujo[i] = new Dibujo(i);
            controlCanal[i] = new ControlCanal(canal[i],dibujo[i]);
            controlMonitor.addObserver(controlCanal[i]);
        }
        
        gbc.gridx=0;gbc.gridy=0;gbc.ipadx = 100;gbc.ipady = 30;
        for (int i=0;i<8;i++){
            gbl.setConstraints(controlCanal[i],gbc);
            panelDibujos.add(controlCanal[i]);
            gbc.gridy=i+1;
        }
        
        gbc.ipadx= 450;gbc.ipady= 30;gbc.gridx=1;gbc.gridy=0;
        for (int i=0;i<8;i++){
            gbl.setConstraints(dibujo[i],gbc);
            panelDibujos.add(dibujo[i]);
            gbc.gridy=i+1;
        }
        gbl.setConstraints(controlMonitor.getPanel(),gbc);
        panelDibujos.add(controlMonitor.getPanel());
    }
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        panelCapturar = new javax.swing.JPanel();
        botonCapturar = new javax.swing.JButton();
        panelModo = new javax.swing.JPanel();
        comboBoxModo = new javax.swing.JComboBox();
        panelFrecuencia = new javax.swing.JPanel();
        comboBoxFrecuencia = new javax.swing.JComboBox();
        panelZoom = new javax.swing.JPanel();
        botonZoomIn = new javax.swing.JButton();
        botonZoomOut = new javax.swing.JButton();
        panelDibujos = new javax.swing.JPanel();
        menu = new javax.swing.JMenuBar();
        menuArchivo = new javax.swing.JMenu();
        jMenuItem55 = new javax.swing.JMenuItem();
        jMenuItem56 = new javax.swing.JMenuItem();
        jMenuItem57 = new javax.swing.JMenuItem();
        jMenuItem58 = new javax.swing.JMenuItem();
        jSeparator13 = new javax.swing.JSeparator();
        jMenuItem59 = new javax.swing.JMenuItem();
        menuEditar = new javax.swing.JMenu();
        jMenuItem60 = new javax.swing.JMenuItem();
        jMenuItem61 = new javax.swing.JMenuItem();
        menuAyuda = new javax.swing.JMenu();
        jMenuItem62 = new javax.swing.JMenuItem();
        jSeparator14 = new javax.swing.JSeparator();
        jMenuItem63 = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Analizador Lógico");

        panelCapturar.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createTitledBorder(""), "Muestras"));

        botonCapturar.setText("Capturar");
        botonCapturar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                botonCapturarActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout panelCapturarLayout = new javax.swing.GroupLayout(panelCapturar);
        panelCapturar.setLayout(panelCapturarLayout);
        panelCapturarLayout.setHorizontalGroup(
            panelCapturarLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelCapturarLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(botonCapturar)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        panelCapturarLayout.setVerticalGroup(
            panelCapturarLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelCapturarLayout.createSequentialGroup()
                .addComponent(botonCapturar)
                .addContainerGap(15, Short.MAX_VALUE))
        );

        panelModo.setBorder(javax.swing.BorderFactory.createTitledBorder("Modo"));

        comboBoxModo.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Síncrono", "Asíncrono" }));

        javax.swing.GroupLayout panelModoLayout = new javax.swing.GroupLayout(panelModo);
        panelModo.setLayout(panelModoLayout);
        panelModoLayout.setHorizontalGroup(
            panelModoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelModoLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(comboBoxModo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        panelModoLayout.setVerticalGroup(
            panelModoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelModoLayout.createSequentialGroup()
                .addComponent(comboBoxModo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(24, Short.MAX_VALUE))
        );

        panelFrecuencia.setBorder(javax.swing.BorderFactory.createTitledBorder("Frecuencia de muestreo"));

        comboBoxFrecuencia.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "50KHz", "100KHz", "150KHz", "200KHz" }));

        javax.swing.GroupLayout panelFrecuenciaLayout = new javax.swing.GroupLayout(panelFrecuencia);
        panelFrecuencia.setLayout(panelFrecuenciaLayout);
        panelFrecuenciaLayout.setHorizontalGroup(
            panelFrecuenciaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelFrecuenciaLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(comboBoxFrecuencia, 0, 125, Short.MAX_VALUE)
                .addContainerGap())
        );
        panelFrecuenciaLayout.setVerticalGroup(
            panelFrecuenciaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelFrecuenciaLayout.createSequentialGroup()
                .addComponent(comboBoxFrecuencia, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(24, Short.MAX_VALUE))
        );

        panelZoom.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createTitledBorder(""), "Zoom"));

        botonZoomIn.setText("In");
        botonZoomIn.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                botonZoomInMouseClicked(evt);
            }
        });

        botonZoomOut.setText("Out");
        botonZoomOut.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                botonZoomOutMouseClicked(evt);
            }
        });

        javax.swing.GroupLayout panelZoomLayout = new javax.swing.GroupLayout(panelZoom);
        panelZoom.setLayout(panelZoomLayout);
        panelZoomLayout.setHorizontalGroup(
            panelZoomLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelZoomLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(botonZoomIn)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(botonZoomOut)
                .addContainerGap())
        );
        panelZoomLayout.setVerticalGroup(
            panelZoomLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelZoomLayout.createSequentialGroup()
                .addGroup(panelZoomLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(botonZoomIn)
                    .addComponent(botonZoomOut))
                .addContainerGap(15, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout panelDibujosLayout = new javax.swing.GroupLayout(panelDibujos);
        panelDibujos.setLayout(panelDibujosLayout);
        panelDibujosLayout.setHorizontalGroup(
            panelDibujosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 545, Short.MAX_VALUE)
        );
        panelDibujosLayout.setVerticalGroup(
            panelDibujosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 397, Short.MAX_VALUE)
        );

        menuArchivo.setText("Archivo");

        jMenuItem55.setText("Nuevo");
        menuArchivo.add(jMenuItem55);

        jMenuItem56.setText("Abrir");
        menuArchivo.add(jMenuItem56);

        jMenuItem57.setText("Guardar");
        menuArchivo.add(jMenuItem57);

        jMenuItem58.setText("Guardar como...");
        menuArchivo.add(jMenuItem58);
        menuArchivo.add(jSeparator13);

        jMenuItem59.setText("Salir");
        menuArchivo.add(jMenuItem59);

        menu.add(menuArchivo);

        menuEditar.setText("Editar");

        jMenuItem60.setText("Opciones");
        menuEditar.add(jMenuItem60);

        jMenuItem61.setText("Preferencias");
        menuEditar.add(jMenuItem61);

        menu.add(menuEditar);

        menuAyuda.setText("Ayuda");

        jMenuItem62.setText("Ayuda");
        menuAyuda.add(jMenuItem62);
        menuAyuda.add(jSeparator14);

        jMenuItem63.setText("Acerca de Analizardor");
        jMenuItem63.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem63ActionPerformed(evt);
            }
        });
        menuAyuda.add(jMenuItem63);

        menu.add(menuAyuda);

        setJMenuBar(menu);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(panelDibujos, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(panelCapturar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelModo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelFrecuencia, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelZoom, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(panelCapturar, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(panelModo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(panelFrecuencia, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(panelZoom, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(panelDibujos, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void jMenuItem63ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem63ActionPerformed

}//GEN-LAST:event_jMenuItem63ActionPerformed

private void botonCapturarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_botonCapturarActionPerformed

}//GEN-LAST:event_botonCapturarActionPerformed

private void botonZoomInMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_botonZoomInMouseClicked
    ControlMonitor.getControlMonitor().zoomTodo(+1);
}//GEN-LAST:event_botonZoomInMouseClicked

private void botonZoomOutMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_botonZoomOutMouseClicked
    ControlMonitor.getControlMonitor().zoomTodo(-1);
}//GEN-LAST:event_botonZoomOutMouseClicked

    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new VentanaPrincipal().setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton botonCapturar;
    private javax.swing.JButton botonZoomIn;
    private javax.swing.JButton botonZoomOut;
    private javax.swing.JComboBox comboBoxFrecuencia;
    private javax.swing.JComboBox comboBoxModo;
    private javax.swing.JMenuItem jMenuItem55;
    private javax.swing.JMenuItem jMenuItem56;
    private javax.swing.JMenuItem jMenuItem57;
    private javax.swing.JMenuItem jMenuItem58;
    private javax.swing.JMenuItem jMenuItem59;
    private javax.swing.JMenuItem jMenuItem60;
    private javax.swing.JMenuItem jMenuItem61;
    private javax.swing.JMenuItem jMenuItem62;
    private javax.swing.JMenuItem jMenuItem63;
    private javax.swing.JSeparator jSeparator13;
    private javax.swing.JSeparator jSeparator14;
    private javax.swing.JMenuBar menu;
    private javax.swing.JMenu menuArchivo;
    private javax.swing.JMenu menuAyuda;
    private javax.swing.JMenu menuEditar;
    private javax.swing.JPanel panelCapturar;
    private javax.swing.JPanel panelDibujos;
    private javax.swing.JPanel panelFrecuencia;
    private javax.swing.JPanel panelModo;
    private javax.swing.JPanel panelZoom;
    // End of variables declaration//GEN-END:variables
    
}
