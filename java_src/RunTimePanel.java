
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.File;
import java.io.InputStreamReader;
import javax.swing.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class RunTimePanel extends JPanel {
    private final JTextArea jcomp1;
    private final JButton jcomp2;
    private final JScrollPane scrollPane;
    private  String command;
    public RunTimePanel(String comand){
        this();
        this.command = comand;
    }
    public RunTimePanel() {
        this.command="";
        jcomp1 = new JTextArea(5, 5);
        jcomp1.setFont(new Font("Ariel", Font.PLAIN, 16));
        jcomp2 = new JButton("View Results");
        jcomp2.setVisible(false);
        jcomp2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String os = System.getProperty("os.name").toLowerCase();
                String pythonExecutable;
                String scriptPath = "graph.py";
                // Assuming the application is run from the project root directory
                File scriptWorkingDir = new File("py_src");

                if (os.contains("win")) {
                    pythonExecutable = "py_src\\myenv\\Scripts\\python.exe";
                } else {
                    pythonExecutable = "py_src/myenv/bin/python";
                }

                ProcessBuilder pb = new ProcessBuilder(pythonExecutable, scriptPath);
                pb.directory(scriptWorkingDir);

                try {
                    pb.start();
                } catch (IOException ex) {
                    System.out.println("Couldn't start process. Please make sure you have created the python virtual environment and installed the dependencies as described in the README.md file.");
                    ex.printStackTrace();
                }
                finally {
                    System.exit(0);
                }
            }
        });
        jcomp1.setEditable(false);

        scrollPane = new JScrollPane(jcomp1);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        setPreferredSize(new Dimension(446, 331));
        setLayout(null);

        add(scrollPane);
        add(jcomp2);

        scrollPane.setBounds(25, 25, 390, 250);
        jcomp2.setBounds(160, 290, 115, 25);

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

                if (command != null && !command.trim().isEmpty()) {
                    // Split the command string into individual arguments
                    String[] args = command.trim().split("\\s+");
                    commandList.addAll(Arrays.asList(args));
                }

                ProcessBuilder pb = new ProcessBuilder(commandList);
                pb.directory(new File(".")); // Sets working dir to project root

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
            finally {
                jcomp2.setVisible(true);
            }
        }).start();
    }
  
}
