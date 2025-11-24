import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class HelpPanel extends JPanel {
    private final JTextArea jcomp1;
    private final JScrollPane scrollPane;
    private final JButton backButton;

    public HelpPanel() {
        jcomp1 = new JTextArea(7, 7);
        jcomp1.setFont(new Font("Ariel", Font.PLAIN, 16));
        jcomp1.setEditable(false);

        scrollPane = new JScrollPane(jcomp1);

        backButton = new JButton("Back");
        backButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFrame frame = (JFrame) SwingUtilities.getWindowAncestor(HelpPanel.this);
                if (frame != null) {
                    frame.getContentPane().removeAll();
                    frame.getContentPane().add(new StartPanel());
                    frame.pack();
                    frame.setVisible(true);
                }
            }
        });

        setPreferredSize(new Dimension(560, 504));
        setLayout(null);

        add(scrollPane);
        add(backButton);

        scrollPane.setBounds(25, 25, 510, 434);
        backButton.setBounds(222, 462, 115, 25);

        startProcess();
    }

    void appendLine(String text) {
        SwingUtilities.invokeLater(() -> {
            jcomp1.append(text + "\n");
            jcomp1.setCaretPosition(jcomp1.getDocument().getLength());
        });
    }

    void startProcess() {
        new Thread(() -> {
            try {
                String executablePath = ".\\build\\program.exe";

                List<String> commandList = new ArrayList<>();
                commandList.add(executablePath);
                commandList.add("--help");

                ProcessBuilder pb = new ProcessBuilder(commandList);
                pb.directory(new File("."));

                Process proc = pb.start();

                try (BufferedReader reader = new BufferedReader(
                        new InputStreamReader(proc.getInputStream()))) {

                    String line;
                    while ((line = reader.readLine()) != null) {
                        this.appendLine(line);
                    }
                }
                proc.waitFor();
            } catch (IOException | InterruptedException e) {
                this.appendLine("Error: " + e.getMessage());
                e.printStackTrace();
            }
        }).start();
    }
}
