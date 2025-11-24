import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class StartPanel extends JPanel {
    private JCheckBox ioInput;
    private JComboBox algorithmInput;
    private JTextField threadsInput;
    private JComboBox timerInput;
    private JLabel jcomp5;
    private JLabel jcomp6;
    private JLabel jcomp7;
    private JLabel jcomp8;
    private JButton start;
    private JButton help;
    private static JFrame frame;
    public StartPanel() {
        String[] algorithmInputItems = {"Sorting", "Prime", "Matrix"};
        String[] timerInputItems = {"clock", "rdtsc"};

        ioInput = new JCheckBox ("I/O");
        algorithmInput = new JComboBox (algorithmInputItems);
        threadsInput = new JTextField (5);
        timerInput = new JComboBox (timerInputItems);
        jcomp5 = new JLabel ("Algorithm");
        jcomp6 = new JLabel ("I/O analysis mode");
        jcomp7 = new JLabel ("Number of Threads");
        jcomp8 = new JLabel ("Timer");
        start = new JButton ("Start benchmark");
        start.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

                String nrThreads;
                try{
                    Integer.parseInt(threadsInput.getText());
                    nrThreads = threadsInput.getText();
                }
                catch(NumberFormatException nfe){
                    nrThreads = "auto";
                }
                String io=ioInput.isSelected()?" --io":"";
                String algorithm=algorithmInput.getSelectedItem().toString().toLowerCase();
                String timer=timerInput.getSelectedItem().toString().toLowerCase();

                String command=" --threads "+nrThreads+" --algorithm "+algorithm+" --timer "+timer+io;

                frame.getContentPane().removeAll();
                frame.getContentPane().add (new RunTimePanel(command));
                frame.pack();
                frame.setVisible (true);
            }
        });
        help = new JButton("Help");
        help.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frame.getContentPane().removeAll();
                frame.getContentPane().add (new HelpPanel());
                frame.pack();
                frame.setVisible (true);
            }
        });
        setPreferredSize (new Dimension (263, 229));
        setLayout (null);

        add (ioInput);
        add (algorithmInput);
        add (threadsInput);
        add (timerInput);
        add (jcomp5);
        add (jcomp6);
        add (jcomp7);
        add (jcomp8);
        add (start);
        add (help);

        ioInput.setBounds (145, 40, 100, 25);
        algorithmInput.setBounds (150, 5, 100, 25);
        threadsInput.setBounds (150, 75, 100, 25);
        timerInput.setBounds (150, 115, 100, 25);
        jcomp5.setBounds (15, 5, 110, 25);
        jcomp6.setBounds (15, 40, 115, 25);
        jcomp7.setBounds (15, 75, 120, 25);
        jcomp8.setBounds (15, 115, 100, 25);
        start.setBounds (15, 175, 130, 25);
        help.setBounds (165, 175, 85, 25);
    }


    public static void main (String[] args) {
        frame = new JFrame ("Benchmark");
        frame.setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add (new StartPanel());
        frame.pack();
        frame.setVisible (true);
    }
}
