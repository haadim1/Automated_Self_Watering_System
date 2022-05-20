package sample;

import javafx.collections.FXCollections;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import sample.SerialJavaFx;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Slider;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.io.IOException;
import java.io.OutputStream;

public class Main extends Application {
    private final static int MAX_POTENTIOMETER_VALUE = 1 << 10;
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        var controller = new DataController();
        var sp = SerialJavaFx.getSerialPort("COM5");
        sp.addDataListener(controller);
        var outputStream = sp.getOutputStream();
        var pane = new BorderPane();

        var slider = new Slider();
        slider.setMin(0.0);
        slider.setMax(100.0);
        var label = new Label();


        slider.setOnMousePressed(value ->  {
            int slidernum = (int) slider.getValue();
            if (slidernum > 50){
                sendtoArduino(outputStream, 1);
            } else {sendtoArduino(outputStream, 2);}
        });

        slider.setOnMouseReleased(value ->  {
            int slidernum = (int) slider.getValue();
            if (slidernum > 50){
                sendtoArduino(outputStream, 1);
            } else {sendtoArduino(outputStream, 2);}
        });

        var button = new Button("Water Pump");
        button.setOnMousePressed(value -> {
            try {
                outputStream.write(255);
            }catch (IOException e){
                e.printStackTrace();
            }
        });
        button.setOnMouseReleased(value -> {

            try {
                outputStream.write(0);
            }catch (IOException e){
                e.printStackTrace();
            }
        });

        var now = System.currentTimeMillis();

        var xAxis = new NumberAxis("time", now, now + 50000, 10000); // creates the x-axis (which automatically updates)
        var yAxis = new NumberAxis("sensor reading", 0, MAX_POTENTIOMETER_VALUE, 10); // creates the y-axis

        var series = new XYChart.Series<>(controller.getDataPoints()); // creates the series (all the data)
        var lineChart = new LineChart<>(xAxis, yAxis, FXCollections.singletonObservableList(series)); // creates the chart
        lineChart.setTitle("Moisture Reading");

        pane.setCenter(lineChart);
        pane.setBottom(button);
        pane.setTop(slider);
        pane.setPadding(new Insets(0, 20, 0, 20));

        var scene = new Scene(pane, 400, 200);

        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void sendtoArduino(OutputStream outputStream, int i) {
        try {
            outputStream.write(i);
        }catch (IOException e){
            e.printStackTrace();
        }
    }
}