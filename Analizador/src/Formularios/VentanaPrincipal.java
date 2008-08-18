package Formularios;

import analizador.ControlCanal;
import analizador.Dibujo;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

public class VentanaPrincipal extends javax.swing.JFrame {
    
    public VentanaPrincipal() {
        initComponents();
        initComponentsPropio();
    }
    
    private void initComponentsPropio() {
        int[] muestras = {0, 1, 0, 1};
        GridBagLayout gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();
        panelDibujos.setLayout(gbl);
                
        dibujo1 = new Dibujo(1,muestras);
        controlCanal1 = new ControlCanal(null,null);
        dibujo2 = new Dibujo(2,muestras);
        controlCanal2 = new ControlCanal(null,null);
        dibujo3 = new Dibujo(3,muestras);
        controlCanal3 = new ControlCanal(null,null);
        dibujo4 = new Dibujo(4,muestras);
        controlCanal4 = new ControlCanal(null,null);
        dibujo5 = new Dibujo(5,muestras);
        controlCanal5 = new ControlCanal(null,null);
        dibujo6 = new Dibujo(6,muestras);
        controlCanal6 = new ControlCanal(null,null);
        dibujo7 = new Dibujo(7,muestras);
        controlCanal7 = new ControlCanal(null,null);
        dibujo8 = new Dibujo(8,muestras);
        controlCanal8 = new ControlCanal(null,null);
        
        gbc.gridx=0;
        gbc.gridy=0;
        gbc.ipadx = 100;
        gbc.ipady = 30;
        gbl.setConstraints(controlCanal1,gbc);
        panelDibujos.add(controlCanal1);
        gbc.gridy=1;
        gbl.setConstraints(controlCanal2,gbc);
        panelDibujos.add(controlCanal2);
        gbc.gridy=2;
        gbl.setConstraints(controlCanal3,gbc);
        panelDibujos.add(controlCanal3);
        gbc.gridy=3;
        gbl.setConstraints(controlCanal4,gbc);
        panelDibujos.add(controlCanal4);
        gbc.gridy=4;
        gbl.setConstraints(controlCanal5,gbc);
        panelDibujos.add(controlCanal5);
        gbc.gridy=5;
        gbl.setConstraints(controlCanal6,gbc);
        panelDibujos.add(controlCanal6);
        gbc.gridy=6;
        gbl.setConstraints(controlCanal7,gbc);
        panelDibujos.add(controlCanal7);
        gbc.gridy=7;
        gbl.setConstraints(controlCanal8,gbc);
        panelDibujos.add(controlCanal8);
        
        gbc.ipadx= 450;
        gbc.ipady= 30;
        gbc.gridx=1;
        gbc.gridy=0;
        gbl.setConstraints(dibujo1,gbc);
        panelDibujos.add(dibujo1);
        gbc.gridy=1;
        gbl.setConstraints(dibujo2,gbc);
        panelDibujos.add(dibujo2);
        gbc.gridy=2;
        gbl.setConstraints(dibujo3,gbc);
        panelDibujos.add(dibujo3);
        gbc.gridy=3;
        gbl.setConstraints(dibujo4,gbc);
        panelDibujos.add(dibujo4);
        gbc.gridy=4;
        gbl.setConstraints(dibujo5,gbc);
        panelDibujos.add(dibujo5);
        gbc.gridy=5;
        gbl.setConstraints(dibujo6,gbc);
        panelDibujos.add(dibujo6);
        gbc.gridy=6;
        gbl.setConstraints(dibujo7,gbc);
        panelDibujos.add(dibujo7);
        gbc.gridy=7;
        gbl.setConstraints(dibujo8,gbc);
        panelDibujos.add(dibujo8);
        
      
        

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
        setResizable(false);

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
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
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
                .addContainerGap(20, Short.MAX_VALUE))
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
                .addContainerGap(20, Short.MAX_VALUE))
        );

        panelZoom.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createTitledBorder(""), "Zoom"));

        botonZoomIn.setText("In");

        botonZoomOut.setText("Out");

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
                .addContainerGap(11, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout panelDibujosLayout = new javax.swing.GroupLayout(panelDibujos);
        panelDibujos.setLayout(panelDibujosLayout);
        panelDibujosLayout.setHorizontalGroup(
            panelDibujosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 545, Short.MAX_VALUE)
        );
        panelDibujosLayout.setVerticalGroup(
            panelDibujosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 332, Short.MAX_VALUE)
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
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(panelCapturar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelModo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelFrecuencia, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(panelZoom, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(panelDibujos, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
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
    
    private Dibujo dibujo1,dibujo2,dibujo3,dibujo4,
            dibujo5,dibujo6,dibujo7,dibujo8;
    private ControlCanal controlCanal1,controlCanal2,
            controlCanal3,controlCanal4,controlCanal5,
            controlCanal6,controlCanal7,controlCanal8;
}
