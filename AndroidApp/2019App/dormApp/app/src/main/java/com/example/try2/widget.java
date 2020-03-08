package com.example.try2;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.Context;
import android.content.Intent;
import android.os.StrictMode;
import android.support.v4.app.INotificationSideChannel;
import android.util.Log;
import android.widget.RemoteViews;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Implementation of App Widget functionality.
 */
public class widget extends AppWidgetProvider {
    public static String text = "original";
    static void updateAppWidget(Context context, AppWidgetManager appWidgetManager,
                                int appWidgetId) {

        Intent intentOB = new Intent(context, widget.class);
        intentOB.setAction("OPENBLINDS");
        PendingIntent pendingIntentOB = PendingIntent.getBroadcast(context,0,intentOB,0);

        Intent intentCB = new Intent(context, widget.class);
        intentCB.setAction("CLOSEBLINDS");
        PendingIntent pendingIntentCB = PendingIntent.getBroadcast(context,0,intentCB,0);

        Intent intentLO = new Intent(context, widget.class);
        intentLO.setAction("LIGHTSON");
        PendingIntent pendingIntentLO = PendingIntent.getBroadcast(context,0,intentLO,0);

        Intent intentLOFF = new Intent(context, widget.class);
        intentLOFF.setAction("LIGHTSOFF");
        PendingIntent pendingIntentLOFF = PendingIntent.getBroadcast(context,0,intentLOFF,0);

        // Construct the RemoteViews object
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.widget);
        views.setOnClickPendingIntent(R.id.button2, pendingIntentOB);
        views.setOnClickPendingIntent(R.id.button4, pendingIntentCB);
        views.setOnClickPendingIntent(R.id.button5, pendingIntentLO);
        views.setOnClickPendingIntent(R.id.button6, pendingIntentLOFF);
        appWidgetManager.updateAppWidget(appWidgetId, views);
        // Instruct the widget manager to update the widget

    }

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
        // There may be multiple widgets active, so update all of them
        for (int appWidgetId : appWidgetIds) {
            updateAppWidget(context, appWidgetManager, appWidgetId);
        }
    }

    @Override
    public void onEnabled(Context context) {
        // Enter relevant functionality for when the first widget is created
    }

    @Override
    public void onDisabled(Context context) {
        // Enter relevant functionality for when the last widget is disabled
    }
    private static HttpURLConnection con;
    @Override
    public void onReceive(Context context, Intent intent)
    {
        super.onReceive(context,intent);
        try {
            if (intent.getAction().equals("OPENBLINDS")) {
                text = "new";
                Log.e("Owen", "ob");

                String url = "http://owenserver.us.to:23655";
                StrictMode.ThreadPolicy policy = new
                        StrictMode.ThreadPolicy.Builder()
                        .permitAll().build();
                StrictMode.setThreadPolicy(policy);


                URL myurl = new URL(url);
                con = (HttpURLConnection) myurl.openConnection();
                con.setDoOutput(true);
                con.setRequestProperty("openBlinds" , "1");
                con.setRequestMethod("POST");

                OutputStreamWriter out = new OutputStreamWriter(
                        con.getOutputStream());
                out.write("set:blindsMoveAllUp=1;");
                out.close();
                con.getInputStream();
                con.disconnect();
            }
            if (intent.getAction().equals("CLOSEBLINDS")) {
                text = "new";
                Log.e("Owen", "cb");

                String url = "http://owenserver.us.to:23655";
                StrictMode.ThreadPolicy policy = new
                        StrictMode.ThreadPolicy.Builder()
                        .permitAll().build();
                StrictMode.setThreadPolicy(policy);


                URL myurl = new URL(url);
                con = (HttpURLConnection) myurl.openConnection();
                con.setDoOutput(true);
                con.setRequestProperty("closeBlinds" , "1");
                con.setRequestMethod("POST");

                OutputStreamWriter out = new OutputStreamWriter(
                        con.getOutputStream());
                out.write("set:blindMoveAllDown=1;");
                out.close();
                con.getInputStream();
                con.disconnect();
            }
            if (intent.getAction().equals("LIGHTSON")) {
                text = "new";
                Log.e("Owen", "LO");

                String url = "http://owenserver.us.to:23655";
                StrictMode.ThreadPolicy policy = new
                        StrictMode.ThreadPolicy.Builder()
                        .permitAll().build();
                StrictMode.setThreadPolicy(policy);


                URL myurl = new URL(url);
                con = (HttpURLConnection) myurl.openConnection();
                con.setDoOutput(true);
                con.setRequestProperty("lightsOn" , "1");
                con.setRequestMethod("POST");

                OutputStreamWriter out = new OutputStreamWriter(
                        con.getOutputStream());
                out.write("multipleChange:bothLights=1;");
                out.close();
                con.getInputStream();
                con.disconnect();
            }
            if (intent.getAction().equals("LIGHTSOFF")) {
                text = "new";
                Log.e("Owen", "LF");

                String url = "http://owenserver.us.to:23655";
                StrictMode.ThreadPolicy policy = new
                        StrictMode.ThreadPolicy.Builder()
                        .permitAll().build();
                StrictMode.setThreadPolicy(policy);


                URL myurl = new URL(url);
                con = (HttpURLConnection) myurl.openConnection();
                con.setDoOutput(true);
                con.setRequestProperty("lightsOff" , "1");
                con.setRequestMethod("POST");

                OutputStreamWriter out = new OutputStreamWriter(
                        con.getOutputStream());
                out.write("multipleChange:bothLights=0;");
                out.close();
                con.getInputStream();
                con.disconnect();            }
        }
        catch (Exception e)
        {
            Log.e("Owen", e.toString());
        }
    }

}

